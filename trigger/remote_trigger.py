#!/usr/bin/env python3
# -*- coding:utf-8 -*-

from datetime import datetime
import subprocess,re

# kill the process on port 5025 to prevent nc address occupied error
popen = subprocess.Popen(['netstat', '-lpn'], shell=False, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
(data, err) = popen.communicate()
pattern = "^tcp.*(?:5025).* (?P<pid>[0-9]*)/.*$"
prog = re.compile(pattern)
for line in data.decode().split('\n'):
    match = re.match(prog, line)
    if match != None:
        pid = match.group('pid')
        subprocess.Popen(['kill', '-9', pid])

while True:
# continually listen on port 5025, if connection is broken, reconnect immediately
    with subprocess.Popen(["nc", "-l", "5025"], stdout=subprocess.PIPE) as netcat:
        print("YUN: Waiting for trigger...")
        message = netcat.stdout.readline().strip().decode("utf-8")

        if (message == "triggered"):
            print("YUN: Triggered! " + str(datetime.now()))

