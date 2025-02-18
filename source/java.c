#include <falso_jni/FalsoJNI_Impl.h>

#include <string.h>
#include <stdio.h>

/*
 * JNI Methods
*/

jstring command(int id, va_list args) {
	// get args from va-list arg
	char *arg = va_arg(args, char *);
	
	char* key = strtok(arg, " ");
	char* value = strtok(NULL, " ");

	if (strstr(key, "storegetstatus")) {
		return (jstring)"2";
	} else if (strstr(key, "storeisrestored")) {
		return (jstring)"true";
	} else if (strstr(key, "storegetprice")) {
		return (jstring)"";
	} else if (strstr(key, "issignedin")) {
		return (jstring)"false";
	} else if (strstr(key, "storeenabled")) {
		return (jstring)"true";
	} else {
		printf("JNI: Method Call: command ~ (%s, %s)\n", key, value);
	}

	return (jstring)"";
}

NameToMethodID nameToMethodId[] = {
	{10, "command", METHOD_TYPE_OBJECT},
};

MethodsBoolean methodsBoolean[] = {};
MethodsByte methodsByte[] = {};
MethodsChar methodsChar[] = {};
MethodsDouble methodsDouble[] = {};
MethodsFloat methodsFloat[] = {};
MethodsInt methodsInt[] = {};
MethodsLong methodsLong[] = {};
MethodsObject methodsObject[] = {
	{ 10, command },
};
MethodsShort methodsShort[] = {};
MethodsVoid methodsVoid[] = {};

/*
 * JNI Fields
*/

// System-wide constant that applications sometimes request
// https://developer.android.com/reference/android/content/Context.html#WINDOW_SERVICE
char WINDOW_SERVICE[] = "window";

// System-wide constant that's often used to determine Android version
// https://developer.android.com/reference/android/os/Build.VERSION.html#SDK_INT
// Possible values: https://developer.android.com/reference/android/os/Build.VERSION_CODES
const int SDK_INT = 19; // Android 4.4 / KitKat

NameToFieldID nameToFieldId[] = {
		{ 0, "WINDOW_SERVICE", FIELD_TYPE_OBJECT }, 
		{ 1, "SDK_INT", FIELD_TYPE_INT },
};

FieldsBoolean fieldsBoolean[] = {};
FieldsByte fieldsByte[] = {};
FieldsChar fieldsChar[] = {};
FieldsDouble fieldsDouble[] = {};
FieldsFloat fieldsFloat[] = {};
FieldsInt fieldsInt[] = {
		{ 1, SDK_INT },
};
FieldsObject fieldsObject[] = {
		{ 0, WINDOW_SERVICE },
};
FieldsLong fieldsLong[] = {};
FieldsShort fieldsShort[] = {};

__FALSOJNI_IMPL_CONTAINER_SIZES
