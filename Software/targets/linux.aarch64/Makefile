export TARGET:=linux.aarch64
export EXECUTABLE:=$(notdir $(realpath $(CURDIR)/../../..))_ARM64
OPENMRNPATH ?= $(shell \
sh -c "if [ \"X`printenv OPENMRNPATH`\" != \"X\" ]; then printenv OPENMRNPATH; \
     elif [ -d /opt/openmrn/src ]; then echo /opt/openmrn; \
     elif [ -d ~/openmrn/src ]; then echo ~/openmrn; \
     elif [ -d ../../../src ]; then echo ../../..; \
     else echo OPENMRNPATH not found; fi" \
)

APP_PATH ?= $(realpath ../..)
-include $(APP_PATH)/config.mk
-include local_config.mk
INCLUDES += -I$(realpath ../../httpd)
SUBDIRS = httpd
include $(OPENMRNPATH)/etc/prog.mk
$(EXECUTABLE)$(EXTENTION): WendellDepot.o

WendellDepot.o: WendellDepot.cxx

main.o: WendellDepot.hxx

WendellDepot.hxx WendellDepot.cxx: ../../../WendellDepot.xml ../../LayoutControlDB2ConfigureCXX.tcl
	tclsh ../../LayoutControlDB2ConfigureCXX.tcl
