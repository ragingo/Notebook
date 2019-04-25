package main

import (
	"fmt"
	"time"
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

// swap
func swap(a *int, b *int) {
	t := *a
	*a = *b
	*b = t
}

// reverse
func reverse(items []int) {
	var j = len(items) - 1
	for i := 0; i < len(items)/2; i++ {
		swap(&items[i], &items[j-i])
	}
}

func seqEquals(items1 []int, items2 []int) bool {
	if len(items1) != len(items2) {
		return false
	}
	for i := 0; i < len(items1); i++ {
		if items1[i] != items2[i] {
			return false
		}
	}
	return true
}

func test1() {

	defer func() { fmt.Println("defer 1") }()

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
	{
		var items1 = seq(1, 9, 1)
		var items2 = seq(1, 9, 1)
		reverse(items2)
		fmt.Printf("reverse(%v) : %v\n", items1, items2)
	}
	{
		var items1 = seq(1, 9, 1)
		var items2 = seq(1, 9, 1)
		fmt.Printf("seqEquals(%v, %v) : %#v\n", items1, items2, seqEquals(items1, items2))
	}
	{
		var items1 = seq(1, 9, 1)
		var items2 = seq(2, 10, 1)
		fmt.Printf("seqEquals(%v, %v) : %#v\n", items1, items2, seqEquals(items1, items2))
	}
	{
		const a = iota
		const b = iota
		const c = iota
		fmt.Printf("ioa : %d, %d, %d\n", a, b, c)

		const (
			d = iota
			e = iota
			f = iota
		)
		fmt.Printf("ioa : %d, %d, %d\n", d, e, f)

		const (
			g = iota
			h
			i
		)
		fmt.Printf("ioa : %d, %d, %d\n", g, h, i)
	}

	{
		defer func() { fmt.Println("defer 2") }()
	}

	defer func() { fmt.Println("defer 3") }()
}

type goroutineInfo struct {
	routine func(state *goroutineState)
}
type goroutineState struct {
	finish bool
}

func test2() {

	var routineInfos []goroutineInfo
	var routineStates []*goroutineState

	routineInfos = append(routineInfos, goroutineInfo{
		func(state *goroutineState) {
			defer func() { fmt.Println("goroutine #0 finish") }()
			fmt.Println("goroutine #0 start")
			time.Sleep(2 * time.Second)
			state.finish = true
		}})
	routineInfos = append(routineInfos, goroutineInfo{
		func(state *goroutineState) {
			defer func() { fmt.Println("goroutine #1 finish") }()
			fmt.Println("goroutine #1 start")
			time.Sleep(5 * time.Second)
			state.finish = true
		}})

	for _, r := range routineInfos {
		var state = new(goroutineState)
		routineStates = append(routineStates, state)
		go r.routine(state)
	}

	for {
		var count = 0
		for _, s := range routineStates {
			if s.finish {
				count++
			}
		}
		if count == len(routineStates) {
			break
		}
	}
}

func main() {

	// test1()
	test2()

}
