#ifndef QDEV_CORE_H
#define QDEV_CORE_H

//
#include "../qemu/typedefs.h"

// 基本的 device
#define TYPE_DEVICE "device"

#define DEVICE_CLASS(klass) OBJECT_CLASS_CHECK(DeviceClass, (klass), TYPE_DEVICE)
typedef enum DeviceCategory {
	DEVICE_CATEGORY_BRIDGE,
	DEVICE_CATEGORY_USB,
	DEVICE_CATEGORY_STORAGE,
	DEVICE_CATEGORY_NETWORK,
	DEVICE_CATEGORY_INPUT,
	DEVICE_CATEGORY_DISPLAY,
	DEVICE_CATEGORY_SOUND,
	DEVICE_CATEGORY_MISC,
	DEVICE_CATEGORY_MAX
} DeviceCategory;


// 採用 static 讓有引用這個 header 的檔案才能看到這個檔案，其他檔案看不到。否則再多 object 檔案下會出錯誤。
// inline 可以讓引用這函數的地方直接替換，減少 linking。畢竟是 header file 又加入 static。
// 在 header 寫 static and inline 都是為了減少連結錯誤。
static inline const char *qdev_category_get_name(DeviceCategory category)
{
	static const char *category_names[DEVICE_CATEGORY_MAX] = {
		[DEVICE_CATEGORY_BRIDGE]  = "Controller/Bridge/Hub",
		[DEVICE_CATEGORY_USB]     = "USB",
		[DEVICE_CATEGORY_STORAGE] = "Storage",
		[DEVICE_CATEGORY_NETWORK] = "Network",
		[DEVICE_CATEGORY_INPUT]   = "Input",
		[DEVICE_CATEGORY_DISPLAY] = "Display",
		[DEVICE_CATEGORY_SOUND]   = "Sound",
		[DEVICE_CATEGORY_MISC]    = "Misc",
	};
	
	return category_names[category];
};

typedef struct DeviceClass {
	/*< private >*/
	ObjectClass parent_class;
	/*< public >*/
	
//	DECLARE_BITMAP(categories, DEVICE_CATEGORY_MAX);
	const char *fw_name;
	const char *desc;
//	Property *props;
	
	/*
	 * Can this device be instantiated with -device / device_add?
	 * All devices should support instantiation with device_add, and
	 * this flag should not exist.  But we're not there, yet.  Some
	 * devices fail to instantiate with cryptic error messages.
	 * Others instantiate, but don't work.  Exposing users to such
	 * behavior would be cruel; clearing this flag will protect them.
	 * It should never be cleared without a comment explaining why it
	 * is cleared.
	 * TODO remove once we're there
	 */
	bool user_creatable;
	bool hotpluggable;
//
//	/* callbacks */
//	void (*reset)(DeviceState *dev);
//	DeviceRealize realize;
//	DeviceUnrealize unrealize;
//
//	/* device state */
//	const struct VMStateDescription *vmsd;
//
//	/* Private to qdev / bus.  */
//	qdev_initfn init; /* TODO remove, once users are converted to realize */
//	qdev_event exit; /* TODO remove, once users are converted to unrealize */
//	const char *bus_type;
} DeviceClass;

struct DeviceState {
	/*< private >*/
	Object parent_obj;
	/*< public >*/
};


// 基本的 bus
#define TYPE_BUS "bus"


#endif
