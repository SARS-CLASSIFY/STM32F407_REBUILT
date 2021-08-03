#include "widget.h"
#include "ui_widget.h"

static int x_label = 0;//设置横坐标初始值
//数据接收 帧头'T'帧尾'P'
static float max_data = 0;//数据最大值
static int connected_flag = 0;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("数据实时监测"));  //设置标题


    myserial = new QSerialPort();
    serial_flag = true;
    start_flag = true;


    setupPlot();//图形界面初始化函数
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btn_open_port_clicked()
{
    if(serial_flag)
    {
        ui->comboBox->setDisabled(true); //禁止修改串口
        myserial->setPortName(ui->comboBox->currentText()); //设置串口号
        myserial->setBaudRate(QSerialPort::Baud9600); //设置波特
        myserial->setDataBits(QSerialPort::Data8); //设置数据位数
        myserial->setParity(QSerialPort::NoParity);//设置奇偶校验
        myserial->setStopBits(QSerialPort::OneStop);//设置停止位
        myserial->setFlowControl(QSerialPort::NoFlowControl);//非流控制
        if(myserial->open(QIODevice::ReadWrite))
        {
            if(connected_flag == 0){//避免重复打开
                connect(myserial,&QSerialPort::readyRead,this,&Widget::AnalyzeData);
                connected_flag = 1;
            }

            mystarttime = QDateTime::currentDateTime();//图像横坐标初始值参考点，读取初始时间
            qDebug()<<"串口打开成功";
        }
        else
        {
            qDebug()<<"串口打开失败";
        }
        ui->btn_open_port->setText("关闭串口");
        serial_flag = false;//串口标志位置失效
    }
    else
    {
        ui->comboBox->setEnabled(true);//串口号下拉按钮使能工作
        myserial->close();
        ui->btn_open_port->setText("打开串口");//按钮显示“打开串口”
        serial_flag = true;//串口标志位置工作
    }
}


void Widget::on_btn_search_port_clicked()
{
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())//读取串口信息
    {
        myserial->setPort(info);//这里相当于自动识别串口号之后添加到了cmb，如果要手动选择可以用下面列表的方式添加进去
        if(myserial->open(QIODevice::ReadWrite))
         {
          ui->comboBox->addItem(myserial->portName());//将串口号添加到cmb
          myserial->close();//关闭串口等待人为(打开串口按钮)打开
         }
     }
}


void Widget::AnalyzeData()
{
    QByteArray mytemp = myserial->readAll();//定义mytemp为串口读取的所有数据
    qDebug()<<"mytemp:"<<mytemp;
    QString StrI1=tr(mytemp.mid(mytemp.indexOf("T")+1,mytemp.indexOf("P")-mytemp.indexOf("T")-1));//自定义了简单协议，通过前面字母读取需要的数据
    if(sizeof(StrI1)>0){
//
//        QString StrI2=tr(mytemp.mid(mytemp.indexOf("H")+1,mytemp.indexOf("I")-mytemp.indexOf("H")-1));
        ui->line_Temp->setText(StrI1);//显示读取温度值
//
//        ui->line_Humi->setText(StrI2);//显示读取湿度值
        float dataI1=StrI1.toFloat();//将字符串转换成float类型进行数据处理

        if(dataI1>=max_data){   //得到数据最大值
            max_data = dataI1;
        }
//
//        float dataI2=StrI2.toFloat();//将字符串转换成float类型进行数据处理

        mycurrenttime = QDateTime::currentDateTime();//获取系统时间


        if(x_label == 101){             //到上限范围则清除图表

           ui->widget_plot->graph(0)->data().data()->clear();
//
//           ui->widget_plot->graph(1)->data().data()->clear();

           ui->widget_plot->replot();
           x_label = 0;

           //最大值处理
           ui->line_Max->setText( QString("%1").arg(max_data));
           max_data = 0;
        }


        ui->widget_plot->graph(0)->addData(x_label,dataI1);//添加数据1到曲线1
//
//        ui->widget_plot->graph(1)->addData(x_label,dataI2);//添加数据1到曲线1

        x_label++;



        ui->widget_plot->replot();//每次画完曲线一定要更新显示
    }
}

void Widget::setupPlot()
{
    //设置曲线一
    ui->widget_plot->addGraph();//添加一条曲线
    QPen pen;
    pen.setWidth(1);//设置画笔线条宽度
    pen.setColor(Qt::blue);
    ui->widget_plot->graph(0)->setPen(pen);//设置画笔颜色
    ui->widget_plot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); //设置曲线画刷背景
    ui->widget_plot->graph(0)->setName("graph1");                        //图例1名称
    ui->widget_plot->graph(0)->setAntialiasedFill(false);
    ui->widget_plot->graph(0)->setLineStyle((QCPGraph::LineStyle)1);//曲线画笔
    ui->widget_plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone,5));//曲线形状

//
//    ui->widget_plot->addGraph();//添加一条曲线
//    pen.setColor(Qt::red);
//    ui->widget_plot->graph(1)->setPen(pen);//设置画笔颜色
//    ui->widget_plot->graph(1)->setBrush(QBrush(QColor(0, 0, 255, 20))); //设置曲线画刷背景
//    ui->widget_plot->graph(1)->setName("graph2");                        //图例2名称
//    ui->widget_plot->graph(1)->setAntialiasedFill(false);
//    ui->widget_plot->graph(1)->setLineStyle((QCPGraph::LineStyle)1);//曲线画笔
//    ui->widget_plot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone,5));//曲线形状

    //设置图表
    ui->widget_plot->xAxis->setLabel(QStringLiteral("时间/s"));//设置x坐标轴名称
    ui->widget_plot->xAxis->setLabelColor(QColor(20,20,20));//设置x坐标轴名称颜色

    ui->widget_plot->xAxis->setRange(0,100);//设定x轴的范围

    ui->widget_plot->yAxis->setLabel(QStringLiteral("data_name"));//设置y坐标轴名称
    ui->widget_plot->yAxis->setLabelColor(QColor(20,20,20));//设置y坐标轴名称颜色

    ui->widget_plot->yAxis->setRange(0,100);//设定y轴范围

    ui->widget_plot->axisRect()->setupFullAxesBox(true);//设置缩放，拖拽，设置图表的分类图标显示位置
    ui->widget_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom| QCP::iSelectAxes);
    ui->widget_plot->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignTop | Qt::AlignRight);//图例显示位置右上
    ui->widget_plot->legend->setVisible(true);//显示图例

    ui->widget_plot->replot();
}

void Widget::on_pushButton_clicked()//退出按钮点击函数
{
    QApplication* app;
    app->quit();
}

void Widget::on_pushButton_2_clicked()
{
    ui->widget_plot->graph(0)->data().data()->clear();
//
//    ui->widget_plot->graph(1)->data().data()->clear();
    ui->widget_plot->replot();
    x_label = 0;
    max_data = 0;
}
