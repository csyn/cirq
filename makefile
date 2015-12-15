CC = gcc
CFLAGS = -O3 -Wall
DEPFLAGS = -MMD -MP -MF $(@:$(BUILDDIR)/%.o=$(DEPDIR)/%.d)
LFLAGS =
SRCS = cirq_test.c
TARGET = cirq_test
RM = rm -rf
MKDIR = mkdir -p
BUILDDIR = build
DEPDIR = $(BUILDDIR)/deps
OBJS = $(SRCS:%.c=$(BUILDDIR)/%.o)
DEPS = $(SRCS:%.c=$(DEPDIR)/%.d)

.PHONY: test clean

test: $(TARGET)
	./$(TARGET)

# link
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $ -o $@ $^ $(LFLAGS)

# compile and/or generate dep files
$(BUILDDIR)/%.o: %.c
	$(MKDIR) $(BUILDDIR)
	$(MKDIR) $(DEPDIR)
	$(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@ $(LFLAGS)

clean:
	$(RM) $(TARGET) $(DEPDIR) $(BUILDDIR)

-include $(DEPS)

