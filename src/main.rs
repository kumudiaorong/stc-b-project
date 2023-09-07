use iced::widget::{self, row, Column};
use iced::{
    executor, window, Application, Command, Element, Length, Settings, Size, Subscription, Theme,
};
use iced_futures::futures::channel::mpsc::{self, Receiver, Sender};
use iced_futures::subscription;
#[derive(Debug, Clone)]
pub enum UpdateMessage {
    SetSource(String),
}
#[derive(Debug, Clone)]
pub enum AppMessage {
    Scan,
    Connect(Sender<UpdateMessage>),
    SourceSelected(String),
    Update(String, String, bool, bool),
}
pub struct App {
    // available_ports: Vec<String>,
    sender: Option<Sender<UpdateMessage>>,
    available_sources: Vec<String>,
    choosed_source: Option<String>,
    info: Vec<(String, String, bool, bool)>,
}
impl App {}
const WIN_INIT_SIZE: iced::Size<u32> = Size {
    width: 300,
    height: 245,
};

impl App {}

impl Application for App {
    type Executor = executor::Default;
    type Message = AppMessage;
    type Theme = Theme;
    type Flags = ();

    fn new(_: Self::Flags) -> (Self, Command<AppMessage>) {
        (
            Self {
                //None,
                available_sources: Vec::new(),
                sender: None,
                choosed_source: None,
                info: Vec::new(),
            },
            window::resize(WIN_INIT_SIZE),
        )
    }

    fn title(&self) -> String {
        String::from("Rating")
    }

    fn update(&mut self, message: AppMessage) -> Command<AppMessage> {
        match message {
            AppMessage::Scan => {
                self.available_sources = match serialport::available_ports() {
                    Ok(v) => v.iter().map(|p| p.port_name.clone()).collect(),
                    Err(_) => Vec::<String>::new(),
                };
                Command::none()
            }
            AppMessage::Connect(sender) => {
                self.sender = Some(sender);
                Command::none()
            }
            AppMessage::SourceSelected(s) => {
                self.choosed_source = Some(s.clone());
                if let Some(sender) = &mut self.sender {
                    if let Err(e) = sender.try_send(UpdateMessage::SetSource(s)) {
                        println!("send failed: {:?}", e)
                    } else {
                        println!("send ok")
                    }
                }
                Command::none()
            }
            AppMessage::Update(rop, temp, hall, vib) => {
                println!(
                    "update: rop={}, temp={}, hall={}, vib={}",
                    rop, temp, hall, vib
                );
                self.info.push((rop, temp, hall, vib));
                if self.info.len() > 10 {
                    self.info.remove(0);
                }
                Command::none()
            }
        }
    }
    fn subscription(&self) -> Subscription<AppMessage> {
        struct SomeSub;
        enum Source {
            Net,
            Serial(Box<dyn serialport::SerialPort>),
            None,
        }
        enum WorkState {
            SetSource,
            Normal,
            TrySend(AppMessage),
        }
        enum State {
            Starting,
            Working(Receiver<UpdateMessage>, Source, WorkState),
        }
        Subscription::batch([
            //timer
            iced::time::every(std::time::Duration::from_millis(1000)).map(|_| AppMessage::Scan),
            subscription::channel(
                std::any::TypeId::of::<SomeSub>(),
                1000,
                |mut output| async move {
                    let mut state = State::Starting;

                    loop {
                        use iced_futures::futures::sink::SinkExt;
                        match &mut state {
                            State::Starting => {
                                // Create channel
                                let (sender, receiver) = mpsc::channel(1000);

                                // Send the sender back to the application
                                // let _ = output.send(AppMessage::Ready(sender)).await;
                                if let Err(_) = output
                                    .send(
                                        // AppMessage::Ready(sender)
                                        AppMessage::Connect(sender),
                                    )
                                    .await
                                {
                                    // log::warn(format!("send ready failed: {:?}", e).as_str());
                                } else {
                                    // We are ready to receive messages
                                    state = State::Working(
                                        receiver,
                                        Source::None,
                                        WorkState::SetSource,
                                    );
                                }
                            }
                            State::Working(recv, src, state) => match state {
                                WorkState::SetSource => {
                                    // Read next input sent from `Application`
                                    use iced_futures::futures::StreamExt;
                                    match recv.select_next_some().await {
                                        UpdateMessage::SetSource(s) => {
                                            if s == "Net" {
                                            } else {
                                                match serialport::new(s, 115200)
                                                    .timeout(std::time::Duration::from_millis(50))
                                                    .open()
                                                {
                                                    Ok(p) => {
                                                        *src = Source::Serial(p);
                                                        *state = WorkState::Normal;
                                                        println!("open serial ok");
                                                    }
                                                    Err(_) => {}
                                                }
                                            }
                                        }
                                    }
                                }
                                WorkState::Normal => match src {
                                    Source::Serial(port) => match port.write(&[0x01]) {
                                        Ok(_) => {
                                            let mut buf = [0 as u8; 4];
                                            match port.read_exact(buf.as_mut()) {
                                                Ok(_) => {
                                                    let rop = format!("{}{}", buf[1], buf[0]);
                                                    let temp = format!("{}", buf[2]);
                                                    let hall = buf[0] & 0x01 == 0x01;
                                                    let vib = buf[0] & 0x02 == 0x02;
                                                    println!(
                                                        "rop={}, temp={}, hall={}, vib={}",
                                                        rop, temp, hall, vib
                                                    );
                                                    *state = WorkState::TrySend(
                                                        AppMessage::Update(rop, temp, hall, vib),
                                                    );
                                                }
                                                Err(_) => {
                                                    *state = WorkState::SetSource;
                                                }
                                            }
                                        }
                                        Err(_) => {
                                            *state = WorkState::SetSource;
                                        }
                                    },
                                    Source::Net => {}
                                    Source::None => {}
                                },
                                WorkState::TrySend(msg) => {
                                    if let Err(_) = output.feed(msg.clone()).await {
                                        println!("send failed: {:?}", msg);
                                        *state = WorkState::SetSource;
                                    } else {
                                        tokio::time::sleep(std::time::Duration::from_millis(1000))
                                            .await;
                                        *state = WorkState::Normal;
                                    }
                                }
                            },
                        }
                    }
                },
            )
            .into(),
        ])
    }
    fn view(&self) -> Element<AppMessage> {
        let ch = widget::pick_list(
            &self.available_sources,
            self.choosed_source.clone(),
            AppMessage::SourceSelected,
        )
        .placeholder("choose a source")
        .width(Length::Fill);
        let col = self
            .info
            .iter()
            .map(|(rop, temp, hall, vib)| {
                row![
                    row!["rop=", widget::text(rop)].spacing(5),
                    row!["temp=", widget::text(temp)].spacing(5),
                    row!["hall=", widget::text(hall)].spacing(5),
                    row!["vib=", widget::text(vib)].spacing(5),
                ]
                .spacing(5)
                .into()
            })
            .collect::<Vec<_>>();
        Column::with_children(vec![
            ch.into(),
            Column::with_children(col).spacing(5).into(),
        ])
        .into()
    }
}
fn main() {
    let _ = App::run(Settings::default());
}
