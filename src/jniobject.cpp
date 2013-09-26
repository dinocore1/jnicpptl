
#include <jnicpptl/jniobject.h>
#include <jnicpptl/jnimethod.h>



/*
template<class T>
JniObject::JniObject(const char* name, JNIEnv* env, const JniMethod<T> &constructor)
 : mClassname(name)
 , mEnv(env)
 , mInstance(NULL)
 , mCachedClassObj(NULL)
{

}
*/

const JniObject makeJniObject(JNIEnv* env, jobject instance)
{
	JniMethod<jstring()> getName("getName", "()Ljava/lang/String;");
	JniString className(env, getName());

	JniObject retval(className, env, instance);
	return retval;
}


