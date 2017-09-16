package main

import "fmt"

type KMP struct {
	F     []int32
	T     []int32
	empty bool
}

// Prepare calculates fail function.
// State machine: s0 is the start state, sn is the terminal state.
// T is template string.
// F is fail function. F[i] represents which state to go when si cannot continue.
// The key property: if si fails, go to max j so that T[0..j-1] == T[i-j..i-1]
func (k *KMP) Prepare(ts string) {
	if ts == "" {
		k.empty = true
		return
	}
	t := k.convert(ts)
	// Avoid additional comparation
	t = append(t, -1)
	n := len(t)

	f := make([]int32, n+1)
	f[0] = -1
	f[1] = 0
	j := int32(0)
	for i := 2; i <= n; i++ {
		for t[i-1] != t[j] && j != 0 {
			j = f[j]
		}
		if t[i-1] == t[j] {
			f[i] = j + 1
			j++
		} else {
			f[i] = 0
		}
	}
	k.F = f
	k.T = t
	k.empty = false
}

func (k *KMP) convert(s string) []rune {
	r := []rune(s)
	n := len(r)
	res := make([]int32, n)
	for i, ch := range r {
		res[i] = int32(ch)
	}
	return res
}

func (k *KMP) MatchAll(in string) []int {
	return k.match(in, true)
}

func (k *KMP) Match(in string) int {
	res := k.match(in, false)
	if len(res) == 0 {
		return -1
	}
	return res[0]
}

func (k *KMP) match(in string, all bool) (firsts []int) {
	firsts = make([]int, 0)
	if k.empty {
		return
	}
	s := k.convert(in)
	t := k.T
	f := k.F
	j := int32(0)
	for i := 0; i < len(s); i++ {
		for s[i] != t[j] && j != 0 {
			j = f[j]
		}
		if s[i] == t[j] {
			j++
		}
		if int(j) == len(t)-1 {
			firsts = append(firsts, i-len(t)+2)
			if !all {
				break
			}
		}
	}
	return
}

func main() {
	kmp := &KMP{}
	kmp.Prepare("abb")
	fmt.Println(kmp.Match("xabbnzbba"))
	fmt.Println(kmp.Match("babodsfabbnsabbbbb"))
	kmp.Prepare("中文")
	fmt.Println(kmp.MatchAll("GO中文"))
	fmt.Println(kmp.MatchAll("我会说中文，但是中文很难"))
}
