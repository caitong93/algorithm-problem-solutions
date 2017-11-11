package btree

import (
	"fmt"
	"sort"
	"strings"
)

// Tutorial http://btechsmartclass.com/DS/U5_T3.html

type Btree struct {
	m    int
	Root *Node
}

type Node struct {
	dat   []KeyValue
	Nodes map[int]*Node
}

func (n *Node) KeyCount() int {
	return len(n.dat)
}

func (n *Node) NodeCount() int {
	return len(n.Nodes)
}

func (n *Node) at(i int) KeyValue {
	return n.dat[i]
}

type KeyValue interface {
	Comparable
	Value() interface{}
	Key() interface{}
}

type Comparable interface {
	LessThan(other interface{}) bool
	Equal(other interface{}) bool
}

func newNode() *Node {
	return &Node{
		dat:   []KeyValue{},
		Nodes: map[int]*Node{},
	}
}

func (b *Btree) insert(n *Node, kv KeyValue) (*Node, *Node, KeyValue) {
	if n == nil {
		leaf := newNode()
		return b.insert(leaf, kv)
	}
	pos := sort.Search(len(n.dat), func(i int) bool {
		return kv.LessThan(n.dat[i])
	})

	if pos < n.KeyCount() && n.at(pos).Equal(kv) {
		n.dat[pos] = kv
		return n, nil, nil
	}

	if len(n.Nodes) == 0 {
		insertKey(n, pos, kv)
	} else {
		ch1, ch2, x := b.insert(n.Nodes[pos], kv)
		if ch2 != nil {
			insertKey(n, pos, x)
			for k, v := range n.Nodes {
				if k > pos {
					delete(n.Nodes, k)
					n.Nodes[k+1] = v
				}
			}
			n.Nodes[pos] = ch1
			n.Nodes[pos+1] = ch2
		} else {
			n.Nodes[pos] = ch1
		}
	}

	return b.Maintain(n)
}

func insertKey(n *Node, pos int, kv KeyValue) {
	var buf []KeyValue
	if pos > 0 {
		buf = n.dat[:pos]
	}
	buf = append(buf, kv)
	buf = append(buf, n.dat[pos:]...)
	n.dat = buf
}

func (t *Btree) Maintain(n *Node) (*Node, *Node, KeyValue) {
	if n.KeyCount() < t.m {
		return n, nil, nil
	}

	mid := t.m / 2
	midVal := n.at(mid)

	ch2 := newNode()
	ch2.dat = n.dat[mid+1:]
	for k, v := range n.Nodes {
		if k > mid {
			ch2.Nodes[k-mid-1] = v
			delete(n.Nodes, k)
		}
	}

	n.dat = n.dat[:mid]
	return n, ch2, midVal
}

func (t *Btree) Insert(kv KeyValue) {
	ch1, ch2, x := t.insert(t.Root, kv)
	if ch2 != nil {
		root, _, _ := t.insert(newNode(), x)
		root.Nodes[0] = ch1
		root.Nodes[1] = ch2
		t.Root = root
	} else {
		t.Root = ch1
	}
}

func (t *Btree) Get(key Comparable) interface{} {
	return t.get(t.Root, key)
}

func (t *Btree) get(n *Node, key Comparable) interface{} {
	if n == nil {
		return nil
	}
	pos := sort.Search(len(n.dat), func(i int) bool {
		return !n.at(i).LessThan(key)
	})

	if pos < n.KeyCount() && n.at(pos).Equal(key) {
		return n.at(pos).Value()
	}

	return t.get(n.Nodes[pos], key)
}

func (t *Btree) Print() {
	if t.Root == nil {
		return
	}
	t.print(-1, 0, t.Root)
}

func stringfyNodeWithID(id int, n *Node) string {
	items := []string{}
	for _, kv := range n.dat {
		items = append(items, fmt.Sprintf("<%v %v>", kv.Key(), kv.Value()))
	}
	return fmt.Sprintf("node %v, %s", id, strings.Join(items, ","))
}

func (t *Btree) print(fa, cur int, n *Node) {
	fmt.Printf("father: %v, %s\n", fa, stringfyNodeWithID(cur, n))
	cnt := cur
	for i := 0; i < n.KeyCount()+1; i++ {
		if son, exist := n.Nodes[i]; exist {
			cnt++
			t.print(cur, cnt, son)
		}
	}
}

func NewBtree(m int) *Btree {
	return &Btree{
		m: m,
	}
}
