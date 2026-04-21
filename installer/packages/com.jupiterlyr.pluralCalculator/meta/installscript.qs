function Component() {
    // 构造函数无操作
}

// 在创建操作时执行
Component.prototype.createOperations = function() {
    component.createOperations();  // 先调用默认操作
    if (systemInfo.productType === "windows") { // 再添加自定义操作
        var targetDir = installer.value("TargetDir");
        var desktopDir = installer.value("DesktopDir");
        var startMenuDir = installer.value("StartMenuDir");
        component.addOperation("CreateShortcut",
            targetDir + "/PluralCalculator.exe",
            desktopDir + "/Plural Calculator.lnk");
        component.addOperation("CreateShortcut",
            targetDir + "/PluralCalculator.exe",
            startMenuDir + "/Plural Calculator.lnk");
    }
}