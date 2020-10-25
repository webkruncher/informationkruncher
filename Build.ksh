#!/usr/bin/env bash
                            
[ -f src.build/informationkruncher ] && rm src.build/informationkruncher
cmake -S src/ -B src.build/ 
cmake --build src.build/    
#src.build/informationkruncher             
[ -f src.build/informationkruncher ] && cat test.txt | src.build/informationkruncher

