#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# This copy of Ice is licensed to you under the terms described in the
# ICE_LICENSE file included in this distribution.
#
# **********************************************************************

import sys, os, signal

path = [ ".", "..", "../..", "../../..", "../../../.." ]
head = os.path.dirname(sys.argv[0])
if len(head) > 0:
    path = [os.path.join(head, p) for p in path]
path = [os.path.abspath(p) for p in path if os.path.exists(os.path.join(p, "demoscript")) ]
if len(path) == 0:
    raise RuntimeError("can't find toplevel directory!")
sys.path.append(os.path.join(path[0]))

from demoscript import Util
from demoscript.Glacier2 import callback

server = Util.spawn('Server.py --Ice.PrintAdapterReady')
server.expect('.* ready')

glacier2 = Util.spawn('glacier2router --Ice.Config=config.glacier2 --Ice.PrintAdapterReady --Glacier2.SessionTimeout=5')
glacier2.expect('Glacier2.Client ready')
glacier2.expect('Glacier2.Server ready')

client = Util.spawn('Client.py')

callback.run(client, server, glacier2)