// online compiler
// https://wandbox.org/permlink/JKHL5WlxEtGeymdw

package main

import (
    "fmt"
)

// sum
func sum(a int, b int) int {
    r := 0
    for i := a; i <= b; i++ {
        r += i
    }
    return r
}

// factrial
func fact(n int) int {
    if n == 1 {
        return 1
    }
    return n * fact(n - 1)
}

// sequence from <= to
func seq(from int, to int, step int) []int {
    slice := []int {}
    for v := from; v <= to; v += step {
        slice = append(slice, v)
    }
    return slice
}

// int array filter
func int_filter(items []int, pred func(int) bool) []int {
    result := []int {}
    for _, item := range items {
        if pred(item) {
            result = append(result, item)
        }
    }
    return result
}

func main() {

    //fmt.Printf("%d", sum(1, 10))
    //fmt.Printf("%d", fact(10))

    for _, v := range int_filter(seq(1, 10, 2), func(v int) bool { return v > 5 }) {
        fmt.Println(v)
    }

}
