LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := reclass-server

LOCAL_CPPFLAGS += -pie -fPIE -ffunction-sections -fdata-sections -fvisibility=hidden
LOCAL_LDFLAGS += -pie -fPIE -Wl,--gc-sections
LOCAL_CFLAGS := -Wno-error=format-security -fpermissive
LOCAL_CFLAGS += -ffunction-sections -fdata-sections -fvisibility=hidden
LOCAL_CFLAGS += -fno-rtti -fno-exceptions

LOCAL_SRC_FILES :=	entry_point.cpp \
					app.cpp \
					logger/logger.cpp \
					network/packet/packet.cpp \
					network/packet/handler/handler.cpp \
					network/packet/request/read_memory_data.cpp \
					network/packet/response/process_list_data.cpp \
					network/packet/response/memory_data.cpp \
					network/socket/socket.cpp \
					network/server/server.cpp \
					android/util.cpp \
					android/syscall.cpp \

LOCAL_C_INCLUDES =		$(LOCAL_PATH)/network/packet/handler/include \
						$(LOCAL_PATH)/network/packet/request/include \
						$(LOCAL_PATH)/network/packet/response/include \
						$(LOCAL_PATH)/network/packet/include \
						$(LOCAL_PATH)/network/socket/include \
						$(LOCAL_PATH)/network/server/include \
						$(LOCAL_PATH)/logger/include \
						$(LOCAL_PATH)/android/include \
						$(LOCAL_PATH)/include \
						

LOCAL_LDLIBS += -llog -lz

include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)