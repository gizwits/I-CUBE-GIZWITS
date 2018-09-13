TOPDIR = $(shell pwd)
GATDIR = $(TOPDIR)/gagent
SUBDIRS = $(TOPDIR)/main $(GATDIR)/http $(GATDIR)/iof/net $(GATDIR)/iof/iofcomm $(GATDIR)/iof/ioffs $(GATDIR)/iof/iofpthread $(GATDIR)/iof/ioftime $(GATDIR)/socket $(GATDIR)/net $(GATDIR)/gatstartup $(GATDIR)/cloud/gserver \
			$(GATDIR)/crypto/aes $(GATDIR)/env $(GATDIR)/cloud/m2m $(GATDIR)/dp $(GATDIR)/core/timer $(GATDIR)/core/utilcode $(GATDIR)/core/nwk \
			$(GATDIR)/local/commlocal $(GATDIR)/local/soclocal

OUTPUTIDR = $(TOPDIR)/objs

CFLAGS += $(foreach dir, $(SUBDIRS), -I$(dir))
CFLAGS += -I$(TOPDIR) -I$(GATDIR)/include/ -I$(GATDIR)/local/commlocal/ -O0 -g

export TOPDIR
export GATDIR
export CFLAGS
export OUTPUTIDR

.PHONY: all
all:
	mkdir -p $(OUTPUTIDR)
	@list='$(SUBDIRS)'; for subdir in $$list; do \
		echo "Clean in $$subdir";\
		cd $$subdir; \
		$(MAKE) ;\
		if [ "$$?" != "0" ]; then\
			echo "\r\n========================= CRITICAL =========================";\
			echo "make $$subdir error!!!";\
			exit 1;\
		fi;\
	done
	gcc $(OUTPUTIDR)/*.o -o $(OUTPUTIDR)/gagent -L/usr/lib64/ -lpthread
	@echo "\r\n========================= Success =========================";
	@echo "GAgent build Success!!!";
	@echo "Run gagent use the following command";
	@echo "sudo "$(OUTPUTIDR)/"gagent\r\n"
clean:
	rm $(OUTPUTIDR)/*
