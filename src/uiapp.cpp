#include "uiapp.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <JavaScriptCore/JSRetainPtr.h>
#include <sstream>
#define WINDOW_WIDTH  1336
#define WINDOW_HEIGHT 768

std::string latestfileName[10];

UIApp::UIApp() {
  ///
  /// Create our main App instance.
  ///
  app_ = App::Create();

  ///
  /// Create a resizable window by passing by OR'ing our window flags with
  /// kWindowFlags_Resizable.
  ///
  window_ = Window::Create(app_->main_monitor(), WINDOW_WIDTH, WINDOW_HEIGHT,
    false, kWindowFlags_Titled | kWindowFlags_Resizable);

  ///
  /// Create our HTML overlay-- we don't care about its initial size and
  /// position because it'll be calculated when we call OnResize() below.
  ///
  overlay_ = Overlay::Create(window_, 1, 1, 0, 0);

  ///
  /// Force a call to OnResize to perform size/layout of our overlay.
  ///
  OnResize(window_.get(), window_->width(), window_->height());

  ///
  /// Load a page into our overlay's View
  ///
  overlay_->view()->LoadURL("file:///app.html");

  ///
  /// Register our MyApp instance as an AppListener so we can handle the
  /// App's OnUpdate event below.
  ///
  app_->set_listener(this);

  ///
  /// Register our MyApp instance as a WindowListener so we can handle the
  /// Window's OnResize event below.
  ///
  window_->set_listener(this);

  ///
  /// Register our MyApp instance as a LoadListener so we can handle the
  /// View's OnFinishLoading and OnDOMReady events below.
  ///
  overlay_->view()->set_load_listener(this);

  ///
  /// Register our MyApp instance as a ViewListener so we can handle the
  /// View's OnChangeCursor and OnChangeTitle events below.
  ///
  overlay_->view()->set_view_listener(this);
}

UIApp::~UIApp() {
}

void UIApp::Run() {
  app_->Run();
}

void UIApp::OnUpdate() {
  ///
  /// This is called repeatedly from the application's update loop.
  ///
  /// You should update any app logic here.
  ///
}

void UIApp::OnClose(ultralight::Window* window) {
  app_->Quit();
}

void UIApp::OnResize(ultralight::Window* window, uint32_t width, uint32_t height) {
  ///
  /// This is called whenever the window changes size (values in pixels).
  ///
  /// We resize our overlay here to take up the entire window.
  ///
  overlay_->Resize(width, height);
}

void UIApp::OnFinishLoading(ultralight::View* caller,
                            uint64_t frame_id,
                            bool is_main_frame,
                            const String& url) {
  ///
  /// This is called when a frame finishes loading on the page.
  ///
}

void UIApp::OnChangeCursor(ultralight::View* caller,
                           Cursor cursor) {
  ///
  /// This is called whenever the page requests to change the cursor.
  ///
  /// We update the main window's cursor here.
  ///
  window_->SetCursor(cursor);
}

void UIApp::OnChangeTitle(ultralight::View* caller,
                          const String& title) {
  ///
  /// This is called whenever the page requests to change the title.
  ///
  /// We update the main window's title here.
  ///
  window_->SetTitle(title.utf8().data());
}


JSValueRef OnButtonSaveClick(JSContextRef ctx, JSObjectRef function,
    JSObjectRef thisObject, size_t argumentCount,
    const JSValueRef arguments[], JSValueRef* exception) {

    OPENFILENAMEA ofn;
    char buf[255] = "\0";
    char filter[] = "All Files\0*.*\0\0";
    char filterExt[][6] = { ".*" };
    char cCustomFilter[256] = "\0\0";
    int nFilterIndex = 0;
    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.hwndOwner = NULL;
    ofn.hInstance = NULL;
    ofn.lpstrFilter = filter;
    ofn.lpstrCustomFilter = cCustomFilter;
    ofn.nMaxCustFilter = 256;
    ofn.nFilterIndex = nFilterIndex;
    ofn.lpstrFile = buf;
    ofn.nMaxFile = 255;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = 0;
    ofn.Flags = OFN_FILEMUSTEXIST;
    ofn.nFileOffset = 0;
    ofn.nFileExtension = 0;
    ofn.lpstrDefExt = NULL;
    ofn.lCustData = NULL;
    ofn.lpfnHook = NULL;
    ofn.lpTemplateName = NULL;

    if (!GetSaveFileNameA(&ofn))
    { 
        return JSValueMakeNull(ctx);
    }
    std::string filename = ofn.lpstrFile;
    std::ofstream wr_file(filename);
    if (!wr_file.is_open()) {
        std::ofstream{ filename };
    } 
    JSStringRef strref = JSValueToStringCopy(ctx, arguments[0], 0);
    size_t bufflen = JSStringGetMaximumUTF8CStringSize(strref);
    char *buffer = new char[bufflen];
    
    JSStringGetUTF8CString(strref, buffer, bufflen);
    wr_file << buffer;
    wr_file.close();
    
    JSStringRelease(strref);
    delete[] buffer;

    int arrNum = JSValueToNumber(ctx, arguments[1], 0);
    latestfileName[arrNum - 1] = filename;
    return JSValueMakeNull(ctx);
}
JSValueRef OnButtonSaveLatestClick(JSContextRef ctx, JSObjectRef function,
    JSObjectRef thisObject, size_t argumentCount,
    const JSValueRef arguments[], JSValueRef* exception) {
    int arrNum = JSValueToNumber(ctx, arguments[1], 0);
    
    std::string filename = latestfileName[arrNum - 1];
    std::ofstream wr_file(filename);
    if (!wr_file.is_open()) {
        std::ofstream{ filename };
    }
    JSStringRef strref = JSValueToStringCopy(ctx, arguments[0], 0);
    size_t bufflen = JSStringGetMaximumUTF8CStringSize(strref);
    char* buffer = new char[bufflen];

    JSStringGetUTF8CString(strref, buffer, bufflen);
    wr_file << buffer;
    wr_file.close();

    JSStringRelease(strref);
    delete[] buffer;

    arrNum = JSValueToNumber(ctx, arguments[1], 0);
    latestfileName[arrNum - 1] = filename;
    
    return JSValueMakeNull(ctx);
}

JSValueRef OnButtonOpenClick(JSContextRef ctx, JSObjectRef function,
    JSObjectRef thisObject, size_t argumentCount,
    const JSValueRef arguments[], JSValueRef* exception) {

    OPENFILENAMEA ofn;
    char buf[255] = "\0";
    char filter[] = "All Files\0*.*\0\0";
    char filterExt[][6] = { ".*" };
    char cCustomFilter[256] = "\0\0";
    int nFilterIndex = 0;
    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.hwndOwner = NULL;
    ofn.hInstance = NULL;
    ofn.lpstrFilter = filter;
    ofn.lpstrCustomFilter = cCustomFilter;
    ofn.nMaxCustFilter = 256;
    ofn.nFilterIndex = nFilterIndex;
    ofn.lpstrFile = buf;
    ofn.nMaxFile = 255;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = 0;
    ofn.Flags = OFN_FILEMUSTEXIST;
    ofn.nFileOffset = 0;
    ofn.nFileExtension = 0;
    ofn.lpstrDefExt = NULL;
    ofn.lCustData = NULL;
    ofn.lpfnHook = NULL;
    ofn.lpTemplateName = NULL;

    if (!GetOpenFileNameA(&ofn))
    {
        return JSValueMakeNull(ctx);
    }


    std::ifstream rd_file(ofn.lpstrFile);
    std::string filecontent;
    std::string curline;
    while (std::getline(rd_file, curline)) {
        
        filecontent += curline;
        filecontent += '\n';
    }
    rd_file.close();

    JSRetainPtr<JSStringRef> str = adopt(
        JSStringCreateWithUTF8CString("writeToCode"));

    JSValueRef func = JSEvaluateScript(ctx, str.get(), 0, 0, 0, 0);
    
    if (JSValueIsObject(ctx, func)) {

        // Cast 'func' to an Object, will return null if typecast failed.
        JSObjectRef funcObj = JSValueToObject(ctx, func, 0);

        // Check if 'funcObj' is a Function and not null
        if (funcObj && JSObjectIsFunction(ctx, funcObj)) {

            // Create a JS string from null-terminated UTF8 C-string, store it
            // in a smart pointer to release it when it goes out of scope.
            JSRetainPtr<JSStringRef> msg =
                adopt(JSStringCreateWithUTF8CString(filecontent.c_str()));
            JSStringRef strref = JSValueToStringCopy(ctx, arguments[0], 0);
            size_t bufflen = JSStringGetMaximumUTF8CStringSize(strref);
            char* buffer = new char[bufflen];

            JSStringGetUTF8CString(strref, buffer, bufflen);
            JSRetainPtr<JSStringRef> msg2 =
                adopt(JSStringCreateWithUTF8CString(buffer));

            JSStringRelease(strref);
            // Create our list of arguments (we only have one)
            const JSValueRef args[] = { JSValueMakeString(ctx, msg.get()), JSValueMakeString(ctx, msg2.get()) };

            // Count the number of arguments in the array.
            size_t num_args = sizeof(args) / sizeof(JSValueRef*);

            // Create a place to store an exception, if any
            JSValueRef exception = 0;

            // Call the ShowMessage() function with our list of arguments.
            JSValueRef result = JSObjectCallAsFunction(ctx, funcObj, 0,
                num_args, args,
                &exception);

            if (exception) {
                // Handle any exceptions thrown from function here.
                MessageBoxA(0, "Something went wrong!", "Exception", MB_OK);
            }

            if (result) {
                // Handle result (if any) here.
            }
        }
    }
    int arrNum = JSValueToNumber(ctx, arguments[1], 0);
    latestfileName[arrNum-1] = ofn.lpstrFile;
    
    return JSValueMakeNull(ctx);
}

JSValueRef OnButtonConsoleOpenClick(JSContextRef ctx, JSObjectRef function,
    JSObjectRef thisObject, size_t argumentCount,
    const JSValueRef arguments[], JSValueRef* exception) {

    OPENFILENAMEA ofn;
    char buf[255] = "\0";
    char filter[] = "All Files\0*.*\0\0";
    char filterExt[][6] = { ".*" };
    char cCustomFilter[256] = "\0\0";
    int nFilterIndex = 0;
    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.hwndOwner = NULL;
    ofn.hInstance = NULL;
    ofn.lpstrFilter = filter;
    ofn.lpstrCustomFilter = cCustomFilter;
    ofn.nMaxCustFilter = 256;
    ofn.nFilterIndex = nFilterIndex;
    ofn.lpstrFile = buf;
    ofn.nMaxFile = 255;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = 0;
    ofn.Flags = OFN_FILEMUSTEXIST;
    ofn.nFileOffset = 0;
    ofn.nFileExtension = 0;
    ofn.lpstrDefExt = NULL;
    ofn.lCustData = NULL;
    ofn.lpfnHook = NULL;
    ofn.lpTemplateName = NULL;

    if (!GetOpenFileNameA(&ofn))
    {
        return JSValueMakeNull(ctx);
    }


    std::ifstream rd_file(ofn.lpstrFile);
    std::string filecontent;
    std::string curline;
    while (std::getline(rd_file, curline)) {

        filecontent += curline;
        filecontent += '\n';
    }
    rd_file.close();

    JSRetainPtr<JSStringRef> str = adopt(
        JSStringCreateWithUTF8CString("writeToConsole"));

    JSValueRef func = JSEvaluateScript(ctx, str.get(), 0, 0, 0, 0);

    if (JSValueIsObject(ctx, func)) {

        // Cast 'func' to an Object, will return null if typecast failed.
        JSObjectRef funcObj = JSValueToObject(ctx, func, 0);

        // Check if 'funcObj' is a Function and not null
        if (funcObj && JSObjectIsFunction(ctx, funcObj)) {

            // Create a JS string from null-terminated UTF8 C-string, store it
            // in a smart pointer to release it when it goes out of scope.
            JSRetainPtr<JSStringRef> msg =
                adopt(JSStringCreateWithUTF8CString(filecontent.c_str()));

            // Create our list of arguments (we only have one)
            const JSValueRef args[] = { JSValueMakeString(ctx, msg.get()) };

            // Count the number of arguments in the array.
            size_t num_args = sizeof(args) / sizeof(JSValueRef*);

            // Create a place to store an exception, if any
            JSValueRef exception = 0;

            // Call the ShowMessage() function with our list of arguments.
            JSValueRef result = JSObjectCallAsFunction(ctx, funcObj, 0,
                num_args, args,
                &exception);

            if (exception) {
                // Handle any exceptions thrown from function here.
                MessageBoxA(0, "Something went wrong!", "Exception", MB_OK);
            }

            if (result) {
                // Handle result (if any) here.
            }
        }
    }

    return JSValueMakeNull(ctx);
}

JSValueRef OnButtonExecuteClick(JSContextRef ctx, JSObjectRef function,
    JSObjectRef thisObject, size_t argumentCount,
    const JSValueRef arguments[], JSValueRef* exception) {


    JSStringRef strref = JSValueToStringCopy(ctx, arguments[0], 0);
    size_t bufflen = JSStringGetMaximumUTF8CStringSize(strref);
    char* buffer = new char[bufflen];
    JSStringGetUTF8CString(strref, buffer, bufflen);
    
    char temp_folder[MAX_PATH];
    GetTempPathA(MAX_PATH, temp_folder);
    std::string batDest = temp_folder;
    batDest += "/ulce_tempexec.bat";
    std::ofstream{ batDest };
    std::ofstream wr_bat(batDest);
    wr_bat << buffer;
    wr_bat.close();

    system("\"%temp%/ulce_tempexec.bat\"");
    
    JSStringRelease(strref);
    delete[] buffer;

    return JSValueMakeNull(ctx);
}

// Use LoadListener::OnDOMReady to wait for the DOM to load.
void UIApp::OnDOMReady(View* caller,
    uint64_t frame_id,
    bool is_main_frame,
    const String& url) {

    // Acquire the JS execution context for the current page.
    //
    // This call will lock the execution context for the current
    // thread as long as the Ref<> is alive.
    //
    RefPtr<JSContext> context = caller->LockJSContext();

    // Get the underlying JSContextRef for use with the
    // JavaScriptCore C API.
    JSContextRef ctx = context.get()->ctx();

    // Create a JavaScript String containing the name of our callback.
    JSStringRef btnsavefunc_name = JSStringCreateWithUTF8CString("OnButtonSaveClick");

    // Create a garbage-collected JavaScript function that is bound to our
    // native C callback 'OnButtonClick()'.
    JSObjectRef btnsavefunc_func = JSObjectMakeFunctionWithCallback(ctx, btnsavefunc_name, OnButtonSaveClick);

    // Get the global JavaScript object (aka 'window')
    JSObjectRef btnsavefunc_globalObj = JSContextGetGlobalObject(ctx);

    // Store our function in the page's global JavaScript object so that it
    // accessible from the page as 'OnButtonClick()'.
    JSObjectSetProperty(ctx, btnsavefunc_globalObj, btnsavefunc_name, btnsavefunc_func, 0, 0);

    // Release the JavaScript String we created earlier.
    JSStringRelease(btnsavefunc_name);

    // Create a JavaScript String containing the name of our callback.
    JSStringRef btnsavelastfunc_name = JSStringCreateWithUTF8CString("OnButtonSaveLatestClick");

    // Create a garbage-collected JavaScript function that is bound to our
    // native C callback 'OnButtonSaveLatestClick()'.
    JSObjectRef btnsavelastfunc_func = JSObjectMakeFunctionWithCallback(ctx, btnsavelastfunc_name, OnButtonSaveLatestClick);

    // Get the global JavaScript object (aka 'window')
    JSObjectRef btnsavelastfunc_globalObj = JSContextGetGlobalObject(ctx);

    // Store our function in the page's global JavaScript object so that it
    // accessible from the page as 'OnButtonClick()'.
    JSObjectSetProperty(ctx, btnsavelastfunc_globalObj, btnsavelastfunc_name, btnsavelastfunc_func, 0, 0);

    // Release the JavaScript String we created earlier.
    JSStringRelease(btnsavelastfunc_name);

    // Create a JavaScript String containing the name of our callback.
    JSStringRef btnopenfunc_name = JSStringCreateWithUTF8CString("OnButtonOpenClick");

    // Create a garbage-collected JavaScript function that is bound to our
    // native C callback 'OnButtonClick()'.
    JSObjectRef btnopenfunc_func = JSObjectMakeFunctionWithCallback(ctx, btnopenfunc_name, OnButtonOpenClick);

    // Get the global JavaScript object (aka 'window')
    JSObjectRef btnopenfunc_globalObj = JSContextGetGlobalObject(ctx);

    // Store our function in the page's global JavaScript object so that it
    // accessible from the page as 'OnButtonClick()'.
    JSObjectSetProperty(ctx, btnopenfunc_globalObj, btnopenfunc_name, btnopenfunc_func, 0, 0);

    // Release the JavaScript String we created earlier.
    JSStringRelease(btnopenfunc_name);

    // CONSOLE FUNCTION

    // Create a JavaScript String containing the name of our callback.
    JSStringRef conbtnexecfunc_name = JSStringCreateWithUTF8CString("OnButtonExecuteClick");

    // Create a garbage-collected JavaScript function that is bound to our
    // native C callback 'OnButtonClick()'.
    JSObjectRef conbtnexecfunc_func = JSObjectMakeFunctionWithCallback(ctx, conbtnexecfunc_name, OnButtonExecuteClick);

    // Get the global JavaScript object (aka 'window')
    JSObjectRef conbtnexecfunc_globalObj = JSContextGetGlobalObject(ctx);

    // Store our function in the page's global JavaScript object so that it
    // accessible from the page as 'OnButtonClick()'.
    JSObjectSetProperty(ctx, conbtnexecfunc_globalObj, conbtnexecfunc_name, conbtnexecfunc_func, 0, 0);

    // Release the JavaScript String we created earlier.
    JSStringRelease(conbtnexecfunc_name);

    // Create a JavaScript String containing the name of our callback.
    JSStringRef conbtnopenfunc_name = JSStringCreateWithUTF8CString("OnButtonConsoleOpenClick");

    // Create a garbage-collected JavaScript function that is bound to our
    // native C callback 'OnButtonClick()'.
    JSObjectRef conbtnopenfunc_func = JSObjectMakeFunctionWithCallback(ctx, conbtnopenfunc_name, OnButtonConsoleOpenClick);

    // Get the global JavaScript object (aka 'window')
    JSObjectRef conbtnopenfunc_globalObj = JSContextGetGlobalObject(ctx);

    // Store our function in the page's global JavaScript object so that it
    // accessible from the page as 'OnButtonClick()'.
    JSObjectSetProperty(ctx, conbtnopenfunc_globalObj, conbtnopenfunc_name, conbtnopenfunc_func, 0, 0);

    // Release the JavaScript String we created earlier.
    JSStringRelease(conbtnopenfunc_name);

}