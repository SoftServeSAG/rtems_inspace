#!/usr/bin/env bash

iptables -t nat -A PREROUTING -p tcp --dport 11411 -j DNAT --to-destination 10.0.0.42:11311
iptables -t nat -A POSTROUTING -j MASQUERADE

# iptables -A PREROUTING -t nat -p tcp -i ppp0 --dport 8001 -j DNAT --to-destination 192.168.1.200:8080
# iptables -A FORWARD -p tcp -d 192.168.1.200 --dport 8080 -m state --state NEW,ESTABLISHED,RELATED -j ACCEPT

# iptables -A PREROUTING -t nat -i eth0 -p tcp --dport 80 -j DNAT --to 192.168.1.2:8080
# iptables -A FORWARD -p tcp -d 192.168.1.2 --dport 8080 -j ACCEPT