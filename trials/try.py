# import example

# print(example.helloworld())

class Node:
    def __init__(self, data):
        self.data = data
        self.next = None

# Create a linked list with three nodes
head = Node(1)
second = Node(2)
third = Node(3)

head.next = second
second.next = third

# Iterate over the linked list
current = head
while current is not None:
    print(current.data, end=' ')
    current = current.next
