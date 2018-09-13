SRCS = $(wildcard *.c)
TMP_OBJS := $(SRCS:%.c=%.o)
OBJS := $(SRCS:%.c=$(OUTPUTIDR)/%.o)

build:$(OBJS)

$(OBJS):$(SRCS)
	gcc -c $(SRCS) $(CFLAGS)
	mv $(TMP_OBJS) $(OUTPUTIDR)