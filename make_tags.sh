#!/bin/bash

ctags -f tags -R ./src
ctags -f tags_include -R /home/ubuntu/mskang/server/lib/json/include/json-c
