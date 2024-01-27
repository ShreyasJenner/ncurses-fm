package main

import (
    "fmt"
    "os"
    "strings"
)

func ReadFile(filename string) []string {
    data, err := os.ReadFile(filename)

    if err != nil {
        fmt.Println(err)
    }

    lines := strings.Split(string(data), "\n")
    return lines
}

func WriteFile(filename, choice string) {
    err := os.WriteFile(filename, []byte(choice), 0644)

    if err != nil {
        fmt.Println(err)
    }
}
