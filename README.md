# EZFrameless

如果出现窗口无法拖动的情况，请在main函数内调用：
QGuiApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings)