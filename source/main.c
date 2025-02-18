#include "utils/init.h"
#include "utils/glutil.h"
#include "utils/logger.h"

#include <psp2/kernel/threadmgr.h>
#include <psp2/kernel/clib.h>

#include <falso_jni/FalsoJNI.h>
#include <falso_ndk/ANativeActivity.h>

#include <so_util/so_util.h>

int _newlib_heap_size_user = 256 * 1024 * 1024;

#ifdef USE_SCELIBC_IO
int sceLibcHeapSize = 4 * 1024 * 1024;
#endif

so_module so_mod;

int main() {
    soloader_init_all();

	int (*ANativeActivity_onCreate)(ANativeActivity *activity, void *savedState,
			size_t savedStateSize) = (void *) so_symbol(&so_mod, "ANativeActivity_onCreate");

	int (*initializeNativeCode)
			(JNIEnv* env, jobject gameActivity, jstring internalDataDir, jstring obbDir,
			jstring externalDataDir, jobject AssetMgr,	jbyteArray savedState)
		= (void*)so_symbol(&so_mod, "Java_com_google_androidgamesdk_GameActivity_initializeNativeCode");

	ANativeActivity *activity = ANativeActivity_create();
	l_info("Created NativeActivity object");
	if (ANativeActivity_onCreate != NULL) {
		ANativeActivity_onCreate(activity, NULL, 0);
		l_info("ANativeActivity_onCreate() passed");

		activity->callbacks->onStart(activity);
		l_info("onStart() passed");

		AInputQueue *aInputQueue = AInputQueue_create();
		activity->callbacks->onInputQueueCreated(activity, aInputQueue);
		l_info("onInputQueueCreated() passed");

		ANativeWindow *aNativeWindow = ANativeWindow_create();
		activity->callbacks->onNativeWindowCreated(activity, aNativeWindow);
		l_info("onNativeWindowCreated() passed");

		activity->callbacks->onWindowFocusChanged(activity, 1);
		l_info("onWindowFocusChanged() passed");

	} else {
		initializeNativeCode(jni, activity, "ux0:data/smash_hit/assets/", "", "", NULL, NULL);
	}
	
	l_info("Main thread shutting down");
	sceKernelExitDeleteThread(0);
}
