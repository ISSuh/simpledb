package simpledb

import (
	"math/rand"
)

type Item struct {
	key   string
	value string
}

type Node struct {
	level    uint32
	prevNode []*Node
	nextNode []*Node
	item     Item
}

func (node *Node) Next(targetLevel uint32) *Node {
}

func (node *Node) Next(targetLevel uint32) *Node {
	if node.level < targetLevel {
		return nil
	}
	return node.nextNode[targetLevel]
}

func (node *Node) Prev(targetLevel uint32) *Node {
	if node.level < targetLevel {
		return nil
	}
	return node.prevNode[targetLevel]
}

func (node *Node) AppendOnLevel(newNode *Node, targetLevel uint32) {
	if node.nextNode != nil {
		node.nextNode[targetLevel].prevNode[targetLevel] = newNode
	}

	newNode.prevNode[targetLevel] = node
	newNode.nextNode[targetLevel] = node.nextNode[targetLevel]

	node.nextNode[targetLevel] = newNode
}

func (node *Node) RemoveOnLevel(targetLevel uint32) {
	if node.nextNode[targetLevel] != nil {
		node.nextNode[targetLevel].prevNode[targetLevel] = node.prevNode[targetLevel]
	}

	if node.prevNode[targetLevel] != nil {
		node.prevNode[targetLevel].nextNode[targetLevel] = node.nextNode[targetLevel]
	}
}

type NodeList struct {
	level uint32
}

type SkipList struct {
	maxLevel   uint32
	randSource rand.Source
	head       []*Node
	tail       []*Node
}
