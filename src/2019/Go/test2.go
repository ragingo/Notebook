package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
)

func DownloadContent(url string, method string) {

	req, _ := http.NewRequest(method, url, nil)
	req.Header.Add("User-Agent", "test")
	client := new(http.Client)

	res, err := client.Do(req)
	if err != nil {
		return
	}
	defer res.Body.Close()

	content, err := ioutil.ReadAll(res.Body)
	if err != nil {
		return
	}

	fmt.Printf("%s", content)
}

func main() {

	DownloadContent("https://google.co.jp", "GET")

}
