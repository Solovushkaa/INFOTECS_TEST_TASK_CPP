.PHONY: all
all: app

.PHONY: liblogger
liblogger:
	$(MAKE) -C liblogger

.PHONY: app
app: liblogger
	$(MAKE) -C app

.PHONY: test
test: liblogger
	$(MAKE) -C liblogger test
	$(MAKE) -C app test

.PHONY: testrun
testrun: test
	$(MAKE) -C liblogger testrun
	$(MAKE) -C app testrun

.PHONY: clean
clean:
	$(MAKE) -C liblogger clean
	$(MAKE) -C app clean

.PHONY: testclean
testclean:
	$(MAKE) -C liblogger testclean
	$(MAKE) -C app testclean

.PHONY: distclean
distclean:
	$(MAKE) -C liblogger distclean
	$(MAKE) -C app distclean
