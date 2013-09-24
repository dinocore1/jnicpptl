
#ifndef JNIOBJECT_H_
#define JNIOBJECT_H_

#include <jni.h>


class JniObject {

private:
	JNIEnv* mEnv;
	jclass classobj;

public:
	const char* classname;

	JniObject(JNIEnv* env, const char* name)
	: mEnv(env)
	, classname(name) {}

	jclass getClass() {
		if(classobj != NULL){
			classobj = mEnv->FindClass(classname);
		}
		return classobj;
	}
};



#endif /* JNIOBJECT_H_ */