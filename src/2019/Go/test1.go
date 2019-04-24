package main

import (
	"fmt"
)

// sum
func sum(items []int) int {
	r := 0
	for _, v := range items {
		r += v
	}
	return r
}

// average
func avg(items []int) float32 {
	r := 0
	for _, v := range items {
		r += v
	}
	return float32(r) / float32(len(items))
}

// factrial
func fact(n int) int {
	if n == 1 {
		return 1
	}
	return n * fact(n-1)
}

// fibonacci
func fib(n int) int {
	if n <= 1 {
		return n
	}
	return fib(n-2) + fib(n-1)
}

// sequence from <= to
func seq(from int, to int, step int) []int {
	slice := []int{}
	for v := from; v <= to; v += step {
		slice = append(slice, v)
	}
	return slice
}

// filter
func filter(items []int, pred func(int) bool) []int {
	result := []int{}
	for _, item := range items {
		if pred(item) {
			result = append(result, item)
		}
	}
	return result
}

// all of
func allOf(items []int, pred func(int) bool) bool {
	for _, item := range items {
		if !pred(item) {
			return false
		}
	}
	return true
}

// any of
func anyOf(items []int, pred func(int) bool) bool {
	var ret = false
	for _, item := range items {
		if pred(item) {
			ret = true
		}
	}
	return ret
}

func main() {

	{
		var n = 5
		fmt.Printf("fact(%d) : %d\n", n, fact(n))
	}
	{
		var n = 7
		fmt.Printf("fib(%d) : %d\n", n, fib(n))
	}
	{
		var items = seq(1, 10, 1)
		fmt.Printf("sum(%v) : %d\n", items, sum(items))
		fmt.Printf("avg(%v) : %f\n", items, avg(items))
	}
	{
		var items = seq(1, 10, 1)
		var pred = func(v int) bool { return v > 5 }
		fmt.Printf("allOf(%v, %#v) : %v\n", items, pred, allOf(items, pred))
		fmt.Printf("anyOf(%v, %#v) : %v\n", items, pred, anyOf(items, pred))
	}
	{
		var items = seq(6, 10, 1)
		var pred = func(v int) bool { return v > 5 }
		fmt.Printf("allOf(%v, %#v) : %v\n", items, pred, allOf(items, pred))
	}
	{
		var items = seq(6, 10, 1)
		var pred = func(v int) bool { return v > 10 }
		fmt.Printf("anyOf(%v, %#v) : %v\n", items, pred, anyOf(items, pred))
	}

}
