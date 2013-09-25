
#include <jnicpptl/jniobject.h>
#include <jnicpptl/jnimethod.h>

JniObject::JniObject(JNIEnv* env, const char* name)
 : mEnv(env)
 , mClassname(name)
{

}

JniObject::JniObject(JNIEnv* env, jobject instance)
 : mEnv(env)
 , mInstance(instance)
{
	JniMethod<jstring()> getName("getName", "()Ljava/lang/String;");
	JniString className(env, getName());
	mClassname = &className;
}

jclass JniObject::getClass()
{
	if(mCachedClassObj != NULL){
		mCachedClassObj = mEnv->FindClass(mClassname.c_str());
	}
	return mCachedClassObj;
}