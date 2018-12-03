#!/bin/bash

ctags -f tags -R ./src
ctags -f tags_include -R /home/ubuntu/mskang/cmd_agent-server/lib/json/include/json-c
