#
# Makefile
#
# Author: Pavel Bednar <bednarpavel@outlook.cz>
#
# This program was created as a part of diploma thesis
# on the topic of deciding WS1S at FIT BUT.
#

BIN_DIR=bin
BUILD_DIR=build
DOC_DIR=doc
DOT_DIR=dot
DOT_NAME=aut
ZIP_NAME=xbedna73_dip

all: both

release:
	mkdir $(BUILD_DIR) $(BIN_DIR) $(DOC_DIR) $(DOT_DIR) && cmake -B$(BUILD_DIR) . && cd $(BUILD_DIR) && $(MAKE)

clean:
	rm -r $(BUILD_DIR) $(BIN_DIR) $(DOC_DIR) $(DOT_DIR)

both:
	$(MAKE) clean && $(MAKE) release

doc:
	doxygen doxy.config

aut:
	dot -Tpng ./dot/$(DOT_NAME).dot -o ./dot/$(DOT_NAME).png

pack:
	zip -r $(ZIP_NAME).zip .
