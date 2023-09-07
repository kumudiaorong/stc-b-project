package main

import (
	"fmt"
	"log"
	"net/http"
	"strconv"
)

func main() {
	rop := ""
	temp := ""
	hall := false
	vib := false
	cnt := 0
	http.HandleFunc("/put", func(w http.ResponseWriter, r *http.Request) {
		rop = r.URL.RawQuery[:3]
		temp = r.URL.RawQuery[3:5]
		hall = r.URL.RawQuery[5] == '1'
		vib = r.URL.RawQuery[6] == '1'
		fmt.Println(cnt, "rop=", rop, " temp=", temp, hall, vib)
		fmt.Fprintf(w, "")
		cnt += 1
	})
	http.HandleFunc("/get", func(w http.ResponseWriter, r *http.Request) {
		w.Header().Add("rop", rop)
		w.Header().Add("temp", temp)
		w.Header().Add("hall", strconv.FormatBool(hall))
		w.Header().Add("vib", strconv.FormatBool(vib))
		fmt.Fprintf(w, "rop=%s,temp=%s,hall=%s,vib=%s", rop, temp, strconv.FormatBool(hall), strconv.FormatBool(vib))
	})
	fmt.Println("start")
	log.Fatal(http.ListenAndServe(":8080", nil))
}
