#!/usr/bin/env python3
# -*- coding:utf-8 -*-

from datetime import datetime
import subprocess

while True:
# continually listen on port 5025, if connection is broken, reconnect immediately
    with subprocess.Popen(["nc", "-l", "5025"], stdout=subprocess.PIPE) as netcat:
        print("YUN: Waiting for trigger...")
        message = netcat.stdout.readline().strip().decode("utf-8")

        if (message == "triggered"):
            print("YUN: Triggered! " + str(datetime.now()))

