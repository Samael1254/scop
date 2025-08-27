NAME = scop

SRCS_DIR = src
HEADERS_DIR = includes
BUILD_DIR = build

SRCS = main.cpp Viewer.cpp

SRCS := $(addprefix $(SRCS_DIR)/, $(SRCS))

OBJS := $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.cpp=.o)))

DEP=$(OBJS:.o=.d)

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++20 -I$(HEADERS_DIR)# -O2

LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

$(NAME): $(OBJS)
	@ echo " \033[33m  Compiling Scop...\033[m"
	@ $(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS)
	@ tput cuu1 && tput el
	@ echo " \033[32m \033[1mScop\033[22m compiled\033[m"

$(BUILD_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@ echo " \033[33m  Compiling $(notdir $@)...\033[m"
	@ mkdir -p $(BUILD_DIR)
	@ $(CXX) $(CXXFLAGS) -c $< -o $@
	@ tput cuu1 && tput el

all: $(NAME)

clean:
	@ rm -f $(OBJS) $(DEP) $(TEST_OBJS) $(TEST_DEP)
	@ rm -rfd $(BUILD_DIR)
	@ echo " \033[32m Object files cleaned\033[m"

fclean: clean
	@ rm -f $(NAME) $(TEST_NAME)
	@ echo " \033[32m \033[1mScop\033[22m cleaned\033[m"

re: fclean all

.PHONY: all fclean re clean

-include $(DEP)
