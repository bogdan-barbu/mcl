# Copyright 2019 Bogdan Barbu
#
# This file is part of MCL.
#
# MCL is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# MCL is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with MCL.  If not, see <https://www.gnu.org/licenses/>.

MCL_SRC_FILES = mcl.c
MCL_OBJ_FILES = $(MCL_SRC_FILES:.c=.o)
MCL_LIB_FILE = libmcl.a

SRC_FILES += $(MCL_SRC_FILES)
LIB_FILES += $(MCL_LIB_FILE)

include makebs/makebs.mk

INSTALL = $(INC_DIR) $(LIB_DIR)

$(LIB_DIR)/$(MCL_LIB_FILE): $(MCL_OBJ_FILES:%=$(OBJ_DIR)/%)
