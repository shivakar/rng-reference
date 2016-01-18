SUBDIRS= MT19937 SplitMix64 TinyMT xorshift128plus xorshift1024star

.PHONY: clean subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

$(SUBDIRS):
	$(MAKE) -C $@
