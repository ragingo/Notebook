package main

import (
	"fmt"
	"net"
	"reflect"
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

// seqEquals
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

// concat
func concat(items1 []int, items2 []int) []int {
	return append(items1, items2...)
}

func test1() {

	defer func() { fmt.Println("defer 1") }()

	// fact
	{
		var n = 5
		fmt.Printf("fact(%d) : %d\n", n, fact(n))
	}
	// fib
	{
		var n = 7
		fmt.Printf("fib(%d) : %d\n", n, fib(n))
	}
	// sum, avg
	{
		var items = seq(1, 10, 1)
		fmt.Printf("sum(%v) : %d\n", items, sum(items))
		fmt.Printf("avg(%v) : %f\n", items, avg(items))
	}
	// allOf, anyOf
	{
		var items = seq(1, 10, 1)
		var pred = func(v int) bool { return v > 5 }
		fmt.Printf("allOf(%v, %#v) : %v\n", items, pred, allOf(items, pred))
		fmt.Printf("anyOf(%v, %#v) : %v\n", items, pred, anyOf(items, pred))
	}
	// allOf
	{
		var items = seq(6, 10, 1)
		var pred = func(v int) bool { return v > 5 }
		fmt.Printf("allOf(%v, %#v) : %v\n", items, pred, allOf(items, pred))
	}
	// anyOf
	{
		var items = seq(6, 10, 1)
		var pred = func(v int) bool { return v > 10 }
		fmt.Printf("anyOf(%v, %#v) : %v\n", items, pred, anyOf(items, pred))
	}
	// reverse
	{
		var items1 = seq(1, 9, 1)
		var items2 = seq(1, 9, 1)
		reverse(items2)
		fmt.Printf("reverse(%v) : %v\n", items1, items2)
	}
	// seqEquals
	{
		var items1 = seq(1, 9, 1)
		var items2 = seq(1, 9, 1)
		fmt.Printf("seqEquals(%v, %v) : %#v\n", items1, items2, seqEquals(items1, items2))
	}
	// seqEquals
	{
		var items1 = seq(1, 9, 1)
		var items2 = seq(2, 10, 1)
		fmt.Printf("seqEquals(%v, %v) : %#v\n", items1, items2, seqEquals(items1, items2))
	}
	// concat
	{
		var items1 = seq(1, 10, 2)
		var items2 = seq(2, 10, 2)
		fmt.Printf("concat(%v, %v) : %#v\n", items1, items2, concat(items1, items2))
	}
	// iota
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
	// slice
	{
		// insert
		var items1 = seq(1, 10, 1)
		items1 = append([]int{99}, items1...)
		fmt.Printf("slice insert %#v\n", items1)

		var items2 = seq(1, 10, 1)
		items2 = append(items2[:0], append([]int{98}, items2[0:]...)...)
		fmt.Printf("slice insert %#v\n", items2)
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

func test3() {

	var routines []func(ch chan bool)

	routines = append(routines,
		func(ch chan bool) {
			defer func() {
				fmt.Println("goroutine #0 finish")
			}()
			fmt.Println("goroutine #0 start")
			time.Sleep(2 * time.Second)
			ch <- true
		})
	routines = append(routines,
		func(ch chan bool) {
			defer func() {
				fmt.Println("goroutine #1 finish")
			}()
			fmt.Println("goroutine #1 start")
			time.Sleep(5 * time.Second)
			ch <- false
		})

	var channels []chan bool

	for _, routine := range routines {
		var channel = make(chan bool)
		channels = append(channels, channel)
		go routine(channel)
	}

	for _, ch := range channels {
		select {
		case r := <-ch:
			fmt.Printf("received! %#v, %#v\n", ch, r)
		}
	}
}

// Integer struct
type Integer struct {
	Value interface{}
}

// ToString a
func (instance *Integer) ToString() string {
	return fmt.Sprintf("%d", instance.Value)
}

// ToHexString a
func (instance *Integer) ToHexString() string {
	switch instance.Value.(type) {
	case int16, uint16:
		return fmt.Sprintf("0x%04x", instance.Value)
	case int, uint, int32, uint32:
		return fmt.Sprintf("0x%08x", instance.Value)
	case int64, uint64:
		return fmt.Sprintf("0x%016x", instance.Value)
	}
	return ""
}

// ToBinaryString a
func (instance *Integer) ToBinaryString() string {
	return fmt.Sprintf("0b%b", instance.Value)
}

// GetInt32 get int32 value from Integer
func (instance *Integer) GetInt32() int32 {
	var v1 = instance.Value
	var v2 = reflect.ValueOf(v1).Int()
	return int32(v2)
}

// GetInt64 get int64 value from Integer
func (instance *Integer) GetInt64() int64 {
	var v1 = instance.Value
	var v2 = reflect.ValueOf(v1).Int()
	return v2
}

// GetBitCount a
func (instance *Integer) GetBitCount() int32 {
	var val = instance.GetInt32()
	var count = int32(0)
	for {
		var d = val >> 1
		var m = val & 1
		val = d
		if m == 1 {
			count++
		}
		if d == 0 {
			break
		}
	}
	return count
}

func pow(b int, e int) int {
	var ret = 1
	for i := 0; i < e; i++ {
		ret *= b
	}
	return ret
}

func str2runes(str string) []rune {
	return []rune(str)
}

func startsWith(str []rune, prefix []rune) bool {
	if len(str) < len(prefix) {
		return false
	}
	for i := 0; i < len(prefix); i++ {
		if str[i] != prefix[i] {
			return false
		}
	}
	return true
}

func bin2dec(bin string) int {
	var bin2 = str2runes(bin)
	if !startsWith(bin2, str2runes("0b")) {
		return 0
	}
	bin2 = bin2[2:]
	var ret = 0
	var len = len(bin2) - 1
	for i := 0; i < len; i++ {
		var bit = int(bin2[i] - rune('0'))
		ret += (bit * pow(2, len-i))
	}
	return ret
}

func rtoi(a rune) int {
	return int(a - rune('0'))
}

func itor(i int) rune {
	return rune(i) + rune('0')
}

func dec2bin(dec int) string {
	var runes []rune
	for {
		var d = dec >> 1
		var m = dec & 1
		runes = append(runes[:0], append([]rune{itor(m)}, runes...)...)
		dec = d
		if d == 0 {
			break
		}
	}
	return "0b" + string(runes)
}

func test4() {
	var i = Integer{Value: 14}
	fmt.Printf("GetInt32: %#v\n", i.GetInt32())
	fmt.Printf("GetInt64: %#v\n", i.GetInt64())
	fmt.Printf("GetBitCount: %#v\n", i.GetBitCount())
	fmt.Printf("ToString: %#v\n", i.ToString())
	fmt.Printf("ToHexString: %#v\n", i.ToHexString())
	fmt.Printf("ToBinaryString: %#v\n", i.ToBinaryString())
	fmt.Printf("pow: %#v\n", pow(2, 8))
	fmt.Printf("bin2dec: %#v\n", bin2dec("0b1110"))
	fmt.Printf("dec2bin: %#v\n", dec2bin(14))
}

// StopWatch struct
type StopWatch struct {
	StartTime   time.Time
	ElapsedTime time.Duration
}

// Start a
func (receiver *StopWatch) Start() {
	receiver.StartTime = time.Now()
}

// Stop a
func (receiver *StopWatch) Stop() {
	receiver.ElapsedTime = time.Since(receiver.StartTime)
}

// Reset a
func (receiver *StopWatch) Reset() {
	receiver.StartTime = time.Time{}
	receiver.ElapsedTime = time.Duration(0)
}

func test5() {
	{
		var start = time.Now()
		time.Sleep(2 * time.Second)
		fmt.Printf("elapsed: %s\n", time.Since(start))
	}
	{
		var sw StopWatch
		sw.Start()
		time.Sleep(3 * time.Second)
		sw.Stop()
		fmt.Printf("start: %s\n", sw.StartTime)
		fmt.Printf("elapsed: %s\n", sw.ElapsedTime)
		sw.Reset()
		fmt.Printf("start: %s\n", sw.StartTime)
		fmt.Printf("elapsed: %s\n", sw.ElapsedTime)
	}
}

func assert(ok bool) {
	if !ok {
		panic("error!")
	}
}

func test6() {

	var done = make(chan bool)

	go func() {

		defer func() {
			println("closed!")
			done <- true
		}()

		var addr *net.TCPAddr
		var conn *net.TCPConn
		var err error

		// 名前解決
		if addr, err = net.ResolveTCPAddr("tcp4", "google.com:80"); err != nil {
			fmt.Printf("%s\n", err)
			return
		}
		assert(addr != nil)

		// 接続
		if conn, err = net.DialTCP("tcp4", nil, addr); err != nil {
			fmt.Printf("%s\n", err)
			return
		}
		assert(conn != nil)
		defer conn.Close()

		const READ_BUF_SIZE = 128
		assert(conn.SetReadBuffer(READ_BUF_SIZE) == nil)

		// 送信
		var req = "GET / HTTP/1.1 \r\n\r\n"
		if _, err = conn.Write([]byte(req)); err != nil {
			fmt.Printf("%s\n", err)
			return
		}

		var buf = make([]byte, READ_BUF_SIZE)
		for {
			var len = 0
			if len, err = conn.Read(buf); err != nil {
				fmt.Printf("%s!!!\n", err)
				break
			}
			if len > 0 {
				fmt.Printf("%s\n", buf)
			}
		}
	}()

	select {
	case <-done:
		print("done.\n")
	}
}

func main() {

	// test1()
	// test2()
	// test3()
	// test4()
	// test5()
	test6()

}
