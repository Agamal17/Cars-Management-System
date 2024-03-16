#include <QApplication>
#include <QDialog>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include <filesystem>
#include <cmath>
#include <fstream>
#include <windows.h>
using namespace std;

class DlgMain : public QDialog
{
public:
    class CarNode
    {
    public:
        string make;
        string model;
        int carsNum;
        CarNode *next;

        CarNode(
            string &mk,
            string &mod,
            int num = 1,
            CarNode *in
            = 0) // Create a CarNode with argument "num" representing quantity to be stored in the inventory
            : make(mk)
            , model(mod)
            , carsNum(num)
            , next(in)
        {}
    };

    CarNode *head, *tail, *p;
    string mk;
    string mod;
    string sNum;

    explicit DlgMain(QWidget *parent = 0);

private:
    QVBoxLayout *lytMain;
    QHBoxLayout *fileLyt;
    QHBoxLayout *addPromptLyt;
    QHBoxLayout *removePromptLyt;
    QHBoxLayout *searchPromptLyt;
    QHBoxLayout *galleryLyt;
    QVBoxLayout *carLyt;

    QPushButton *right;
    QPushButton *add;
    QPushButton *remove;
    QPushButton *search;
    QPushButton *browse;

    QLineEdit *addMake;
    QLineEdit *addModel;
    QLineEdit *addNum;
    QLineEdit *removeMake;
    QLineEdit *removeModel;
    QLineEdit *searchMake;
    QLineEdit *searchModel;
    QLineEdit *filePath;
    QLabel *file;
    QLabel *makeLbl;
    QLabel *modelLbl;
    QLabel *imgLbl;
    QPixmap img;
    QLabel *availableLbl;
    QMessageBox *message;

public slots:
    void finsert(); // insert Car's data with argument "num" representing quantity to be stored in the inventory
    void fsearch();
    void fremove();
    void fdisplay();
    void insertion();
    void insertion_args(string, string, string);
private slots:
    void fbrowse();
    void fright();
};

DlgMain::DlgMain(QWidget *parent)
    : QDialog(parent)
    , head(0)
    , tail(0)
    , p(0)
{
    QFile styleFile("./assets/SyNet.qss");
    styleFile.open(QFile::ReadOnly);
    QString style(styleFile.readAll());
    setStyleSheet(style);
    setFixedSize(500, 500);
    setWindowTitle("Cars");
    setWindowIcon(QIcon("./assets/icon.png"));
    message = new QMessageBox();

    lytMain = new QVBoxLayout(this);
    galleryLyt = new QHBoxLayout;
    fileLyt = new QHBoxLayout;
    addPromptLyt = new QHBoxLayout;
    removePromptLyt = new QHBoxLayout;
    searchPromptLyt = new QHBoxLayout;
    carLyt = new QVBoxLayout;
    lytMain->addLayout(fileLyt);
    lytMain->addLayout(addPromptLyt);
    lytMain->addLayout(removePromptLyt);
    lytMain->addLayout(searchPromptLyt);
    lytMain->addLayout(galleryLyt);

    file = new QLabel("Find file to read from: ");
    filePath = new QLineEdit;
    browse = new QPushButton("Browse");
    connect(browse, &QPushButton::clicked, this, &DlgMain::fbrowse);
    addMake = new QLineEdit;
    addMake->setPlaceholderText("Make");
    addModel = new QLineEdit;
    addModel->setPlaceholderText("Model");
    addNum = new QLineEdit;
    addNum->setPlaceholderText("Quantity (1)");
    add = new QPushButton("Add Car");
    connect(add, &QPushButton::clicked, this, &DlgMain::insertion);
    removeMake = new QLineEdit;
    removeMake->setPlaceholderText("Make");
    removeModel = new QLineEdit;
    removeModel->setPlaceholderText("Model");
    remove = new QPushButton("Remove Car");
    connect(remove, &QPushButton::clicked, this, &DlgMain::fremove);
    searchMake = new QLineEdit;
    searchMake->setPlaceholderText("Make");
    searchModel = new QLineEdit;
    searchModel->setPlaceholderText("Model");
    search = new QPushButton("Search Car");
    connect(search, &QPushButton::clicked, this, &DlgMain::fsearch);
    right = new QPushButton(">");
    right->setFixedHeight(50);
    connect(right, &QPushButton::clicked, this, &DlgMain::fright);

    makeLbl = new QLabel;
    modelLbl = new QLabel;
    imgLbl = new QLabel;
    availableLbl = new QLabel;
    makeLbl->setText(QString());
    modelLbl->setText(QString());
    imgLbl->setText(QString());
    availableLbl->setText(QString());
    carLyt->addWidget(makeLbl);
    carLyt->addWidget(modelLbl);
    carLyt->addWidget(imgLbl);
    carLyt->addWidget(availableLbl);

    fileLyt->addWidget(file);
    fileLyt->addWidget(filePath);
    fileLyt->addWidget(browse);

    addPromptLyt->addWidget(addMake);
    addPromptLyt->addWidget(addModel);
    addPromptLyt->addWidget(addNum);
    addPromptLyt->addWidget(add);
    removePromptLyt->addWidget(removeMake);
    removePromptLyt->addWidget(removeModel);
    removePromptLyt->addWidget(remove);
    searchPromptLyt->addWidget(searchMake);
    searchPromptLyt->addWidget(searchModel);
    searchPromptLyt->addWidget(search);

    galleryLyt->addStretch(50);
    galleryLyt->addLayout(carLyt);
    galleryLyt->addStretch(30);
    galleryLyt->addWidget(right, 30);
}

string upScale(string x)
{ // Function for making all inputs Upper-case to avoid Case-sensitive errors
    int xl = x.length();
    for (int i = 0; i < xl; i++)
        x[i] = toupper(x[i]);
    return x;
}

void DlgMain::insertion_args(string m, string mo, string n = "1")
{
    mk = m;
    mod = mo;
    sNum = n;
    finsert();
}

void DlgMain::insertion()
{
    int counter;
    if (addMake->text().isEmpty()) {
        message->setText(QString("Add a Make"));
        message->exec();
        return;
    }
    mk = addMake->text().toStdString();
    counter = mk.length();
    for (int i = mk.length() - 1; i >= 0; i--) {
        if (mk[i] == ' ')
            counter--;
        else if (mk[i] != ' ')
            break;
    }
    mk = mk.substr(0, counter);
    if (addModel->text().isEmpty()) {
        message->setText(QString("Add a Model"));
        message->exec();
        return;
    }
    mod = addModel->text().toStdString();
    counter = mod.length();
    for (int i = mod.length() - 1; i >= 0; i--) {
        if (mod[i] == ' ')
            counter--;
        else if (mod[i] != ' ')
            break;
    }
    mod = mod.substr(0, counter);
    if (addNum->text().isEmpty())
        addNum->setText("1");
    sNum = addNum->text().toStdString();
    counter = sNum.length();
    for (int i = sNum.length() - 1; i >= 0; i--) {
        if (sNum[i] == ' ')
            counter--;
        else if (sNum[i] != ' ')
            break;
    }
    sNum = sNum.substr(0, counter);
    addMake->clear();
    addModel->clear();
    addNum->clear();
    finsert();
}

void fetchImage(string mk, string mod)
{
    for (int i = 0; i < mk.length(); i++)
        if (mk[i] == ' ')
            mk[i] = '-';
    for (int i = 0; i < mod.length(); i++)
        if (mod[i] == ' ')
            mod[i] = '-';
    filesystem::path current_path = std::filesystem::current_path();
    string cmd = "python " + current_path + " " + mk + "-" + mod;
    system(cmd.c_str());
    Sleep(1500);
}

void DlgMain::finsert()
{
    fetchImage(mk, mod);
    int num = stoi(sNum);
    mk = upScale(mk);
    mod = upScale(mod);
    if (num < 1) {
        message->setText(QString("Enter a Valid Quantity"));
        message->exec();
        mk = mod = "";
        return;
    } else if (head == 0) {
        head = tail = new CarNode(mk, mod, num);
        p = head;
        fdisplay();
    } else {
        CarNode *prev = 0;
        p = head;
        for (; p != 0 && mk >= p->make;
             p = p->next) { // iterates to find the place to put the car's data
            prev = p;
            if (mk == p->make) { // if car's make was found:
                if (mod
                    == p->model) { // if car's make and model is found, increase its number by num
                    (p->carsNum) += num;
                    fdisplay();
                    mk = mod = "";
                    return;
                } else if (mod
                           > p->model) // if make was equal but model was greater, we iterate to find the place to put the model's node in.
                    while (
                        p != 0 && mk == p->make
                        && mod > p->model) { // loop is broken if make was not found again, so it puts the node after the last node of the car with the make,
                        prev = p; // or if model was found to be less than a model with the same make of the car, or if we iterated to the end of list and found nothing.
                        p = p->next;
                    }
                break;
            }
        }
        if (prev == 0) { // to handle if the desired car's data is to be added before head of list
            CarNode *tmp = head;
            head = new CarNode(mk, mod, num);
            p = head;
            fdisplay();
            head->next = tmp;
        } else if (prev
                   == tail) { // to handle if the desired car's data is to be inserted after the tail of list
            prev->next = new CarNode(mk, mod, num);
            tail = prev->next;
            p = tail;
            fdisplay();
        } else {
            prev->next = new CarNode(mk,
                                     mod,
                                     num); // to handle normally inserting a car node between two nodes
            prev->next->next = p;
            p = prev->next;
            fdisplay();
        }
    }
    mk = mod = sNum = "";
}

void DlgMain::fsearch()
{
    int counter;
    mk = searchMake->text().toStdString();
    counter = mk.length();
    for (int i = mk.length() - 1; i >= 0; i--) {
        if (mk[i] == ' ')
            counter--;
        else if (mk[i] != ' ')
            break;
    }
    mk = mk.substr(0, counter);
    mod = searchModel->text().toStdString();
    counter = mod.length();
    for (int i = mod.length() - 1; i >= 0; i--) {
        if (mod[i] == ' ')
            counter--;
        else if (mod[i] != ' ')
            break;
    }
    mod = mod.substr(0, counter);
    searchMake->clear();
    searchModel->clear();
    mk = upScale(mk);
    mod = upScale(mod);
    for (p = head; p != 0; p = p->next)
        if (p->make == mk && p->model == mod) {
            fdisplay();
            p = head;
            mk = mod = "";
            return;
        }
    message->setText(QString("Not Found"));
    message->exec();
    mk = mod = "";
}

void DlgMain::fremove()
{
    int counter;
    mk = removeMake->text().toStdString();
    counter = mk.length();
    for (int i = mk.length() - 1; i >= 0; i--) {
        if (mk[i] == ' ')
            counter--;
        else if (mk[i] != ' ')
            break;
    }
    mk = mk.substr(0, counter);
    mod = removeModel->text().toStdString();
    counter = mod.length();
    for (int i = mod.length() - 1; i >= 0; i--) {
        if (mod[i] == ' ')
            counter--;
        else if (mod[i] != ' ')
            break;
    }
    mod = mod.substr(0, counter);
    removeMake->clear();
    removeModel->clear();
    mk = upScale(mk);
    mod = upScale(mod);
    if (head == 0) { // if list is already Empty
        message->setText(QString("No Cars to Remove"));
        message->exec();
        p = head;
        mk = mod = "";
        return;
    }
    p = head;
    for (CarNode *prev = 0; p != 0; prev = p, p = p->next)
        if (p->make == mk && p->model == mod) { // if car is found
            if (p->carsNum > 1) {               // if carsNum is more than one, decrement it.
                (p->carsNum)--;
                message->setText(QString("Removed!"));
                message->exec();
                fdisplay();
                mk = mod = "";
                return;
            } else {                             // if carsNum was equal to one
                if (head == tail && p == head) { // if data was found at the only node in the list
                    head = tail = 0;
                } else if (p == tail) { // if data was found at the end of list
                    prev->next = 0;
                    tail = prev;
                } else if (p == head)
                    head = p->next; // if data was found at head of list
                else
                    prev->next
                        = p->next; // if none of these special cases occured, this is the normal procedure
                delete p;
                message->setText(QString("Removed!"));
                message->exec();
                p = head;
                fdisplay();
                mk = mod = "";
                return;
            }
        }
    message->setText(QString(
        "Not Found")); // if loop finished without interference from inside, print that the car is not found
    message->exec();
    p = head;
    mk = mod = "";
}

void DlgMain::fbrowse()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Cars File"));
    string suffix;
    for (int i = fileName.length() - 1, c = 0; c < 3; i--, c++) {
        suffix = fileName.toStdString()[i] + suffix;
    }
    if (suffix != "txt") {
        message->setText(QString("File is not .txt"));
        message->exec();
        return;
    }

    ifstream file(fileName.toStdString());
    string input, mk, mod;

    if (!file.is_open()) {
        message->setText(QString("File Couldn't Open"));
        message->exec();
        return;
    }

    filePath->insert(fileName);
    filePath->setDisabled(true);
    browse->setDisabled(true);

    getline(file, input);
    int x = stoi(input);

    int length;
    int num = 0;
    while (x--) {
        getline(file, input);
        length = input.length();
        for (int i = 0; i < length; i++) {
            if (input[i] == '|') {
                for (int j = 0; j < i; j++)
                    mk += input[j];
                for (int j = i + 1; input[j] != ':'; j++)
                    mod += input[j];
            } else if (input[i] == ':') {
                for (int j = length - 1, c = 0; j > i; j--, c++)
                    if (input[j] > 47
                        && input[j]
                               < 58) //for ensuring that the characters after ":" in the file are numbers.
                        num = num
                              + ((input[j] - 48)
                                 * pow(10,
                                       c)); //reading numbers from right to left with the "c" variable acting as power for the radix 10
                break;
            }
        }
        insertion_args(mk, mod, to_string(num));
        mk = mod = "";
        num = 0;
    }
}

void DlgMain::fdisplay()
{
    if (p != 0) {
        makeLbl->setText(QString((p->make).c_str()));
        modelLbl->setText(QString((p->model).c_str()));
        string mkFetch = p->make;
        string modFetch = p->model;
        for (int i = 0; i < mkFetch.length(); i++)
            if (mkFetch[i] == ' ')
                mkFetch[i] = '-';
        for (int i = 0; i < modFetch.length(); i++)
            if (modFetch[i] == ' ')
                modFetch[i] = '-';

        filesystem::path current_path = std::filesystem::current_path();
        string path = current_path + "/images/" + mkFetch + '-' + modFetch;
        img = QPixmap(path.c_str()).scaled(100, 100);
        imgLbl->setPixmap(img);
        availableLbl->setText(to_string(p->carsNum).c_str());
    } else {
        message->setText(QString("No Cars to Display"));
        message->exec();
    }
}

void DlgMain::fright()
{
    p = p == tail ? head : p->next;
    fdisplay();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    DlgMain prg;
    prg.show();
    return app.exec();
}
