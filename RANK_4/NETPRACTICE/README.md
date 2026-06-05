# NetPractice

> Rank 04 · École 42 Common Core — *TCP/IP addressing & subnetting*

## Why there is no source code here
NetPractice is **not a programming project**. It is a hands-on **networking
exercise** completed inside a web-based simulator provided by 42. There is no
program to write — you configure virtual networks until every host can reach the
others. The only artifacts you can keep are the **saved configurations of each
level**, which is exactly what this folder contains: `level1.json` …
`level10.json`, one solved configuration per level.

## What the project was
Ten levels of increasing difficulty in which you must make small networks
communicate by correctly filling in:
- **IP addresses**,
- **subnet masks** (CIDR / dotted-decimal),
- **routing table entries** (default gateways and routes).

Concepts practised: the TCP/IP and OSI models, IPv4 address classes, the
network-vs-host split via the subnet mask (bitwise AND), broadcast/network
addresses, switches vs routers, and static routing.

## How it was completed
For each level the topology is analysed: which interfaces share a subnet, what
mask makes the required hosts (and only those) fall in the same network, and what
gateway/route lets traffic cross between subnets. The validated answers were saved
as the `levelN.json` files kept here.

## Constraints (2025 subject)
- 10 levels; each must be solved so that all the required machines can
  communicate, **without breaking** any constraint shown on screen.
- No invalid addresses (a host cannot use the network or broadcast address;
  masks must be contiguous), and routes must be consistent.

*Subject reference: 42 `NetPractice` subject (2024–2025 edition).*
