from PySide6.QtWidgets import QHBoxLayout, QVBoxLayout, QLabel, QLineEdit

def setParameter(text):
    settings.setValue("a", text)

#print(settings.value("title"))
widget = task.parametersWidget()
vLayout = QVBoxLayout()

hLayout = QHBoxLayout()
label = QLabel("Parameter A: ")
lineEdit = QLineEdit(settings.value("a"))
lineEdit.textChanged.connect(setParameter)

hLayout.addWidget(label)
hLayout.addWidget(lineEdit)

vLayout.addItem(hLayout)
vLayout.addStretch()
widget.setLayout(vLayout)