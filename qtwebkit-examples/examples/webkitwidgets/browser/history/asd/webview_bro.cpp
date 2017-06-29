/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "browserapplication.h"
#include "browsermainwindow.h"
#include "cookiejar.h"
#include "downloadmanager.h"
#include "networkaccessmanager.h"
#include "tabwidget.h"
#include "webview.h"
#include <string.h>

#include <QtGui/QClipboard>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QtGui/QMouseEvent>

#include <QWebHitTestResult>

#ifndef QT_NO_UITOOLS
#include <QtUiTools/QUiLoader>
#endif  //QT_NO_UITOOLS

#include <QtCore/QDebug>
#include <QtCore/QBuffer>

#include <QTimer>

long WebView::abc = 0;

WebPage::WebPage(QObject *parent)
    : QWebPage(parent)
    , m_keyboardModifiers(Qt::NoModifier)
    , m_pressedButtons(Qt::NoButton)
    , m_openInNewTab(false)
{
    setNetworkAccessManager(BrowserApplication::networkAccessManager());
    connect(this, SIGNAL(unsupportedContent(QNetworkReply*)),
            this, SLOT(handleUnsupportedContent(QNetworkReply*)));
}

BrowserMainWindow *WebPage::mainWindow()
{
    QObject *w = this->parent();
    while (w) {
        if (BrowserMainWindow *mw = qobject_cast<BrowserMainWindow*>(w))
            return mw;
        w = w->parent();
    }
    return BrowserApplication::instance()->mainWindow();
}

bool WebPage::acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, NavigationType type)
{
    // ctrl open in new tab
    // ctrl-shift open in new tab and select
    // ctrl-alt open in new window
    if (type == QWebPage::NavigationTypeLinkClicked
        && (m_keyboardModifiers & Qt::ControlModifier
            || m_pressedButtons == Qt::MidButton)) {
       // printf("webivew.cpp ~!~!~!~!~ \n");
        bool newWindow = (m_keyboardModifiers & Qt::AltModifier);
        WebView *webView;
        if (newWindow) {
            BrowserApplication::instance()->newMainWindow();
            BrowserMainWindow *newMainWindow = BrowserApplication::instance()->mainWindow();
            webView = newMainWindow->currentTab();
            newMainWindow->raise();
            newMainWindow->activateWindow();
            webView->setFocus();
        } else {
            bool selectNewTab = (m_keyboardModifiers & Qt::ShiftModifier);
            webView = mainWindow()->tabWidget()->newTab(selectNewTab);
        }
        webView->load(request);
        m_keyboardModifiers = Qt::NoModifier;
        m_pressedButtons = Qt::NoButton;
        return false;
    }
    if (frame == mainFrame()) {
int tab_index;
TabWidget* temp_tabWidget = mainWindow()->tabWidget();
 WebView *webView;
webView =temp_tabWidget->currentWebView();
tab_index =temp_tabWidget->currentIndex();

//if(tab_index==5){
	//cout<<webView->url().toString().toUtf8().data()<<"############  "<<tab_index <<endl;
	//cout<< frame -> url().toString().toUtf8().data()<<endl<<endl;
// }

        m_loadingUrl = request.url();
        emit loadingUrl(m_loadingUrl);
    }
    return QWebPage::acceptNavigationRequest(frame, request, type);
}

QWebPage *WebPage::createWindow(QWebPage::WebWindowType type)
{
    Q_UNUSED(type);
    if (m_keyboardModifiers & Qt::ControlModifier || m_pressedButtons == Qt::MidButton)
        m_openInNewTab = true;
    if (m_openInNewTab) {
        m_openInNewTab = false;
        return mainWindow()->tabWidget()->newTab()->page();
    }
    BrowserApplication::instance()->newMainWindow();
    BrowserMainWindow *mainWindow = BrowserApplication::instance()->mainWindow();
    return mainWindow->currentTab()->page();
}

#if !defined(QT_NO_UITOOLS)
QObject *WebPage::createPlugin(const QString &classId, const QUrl &url, const QStringList &paramNames, const QStringList &paramValues)
{
    Q_UNUSED(url);
    Q_UNUSED(paramNames);
    Q_UNUSED(paramValues);
    QUiLoader loader;
    return loader.createWidget(classId, view());
}
#endif // !defined(QT_NO_UITOOLS)

void WebPage::handleUnsupportedContent(QNetworkReply *reply)
{
    //cout<<"WebPage::handleUnsupportedContent(QNetworkReply *reply)~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    QString errorString = reply->errorString();

    if (m_loadingUrl != reply->url()) {
        // sub resource of this page
        qWarning() << "Resource" << reply->url().toEncoded() << "has unknown Content-Type, will be ignored.";
        reply->deleteLater();
        return;
    }

    if (reply->error() == QNetworkReply::NoError && !reply->header(QNetworkRequest::ContentTypeHeader).isValid()) {
        errorString = "Unknown Content-Type";
    }

    QFile file(QLatin1String(":/notfound.html"));
    bool isOpened = file.open(QIODevice::ReadOnly);
    Q_ASSERT(isOpened);
    Q_UNUSED(isOpened)

    QString title = tr("Error loading page: %1").arg(reply->url().toString());
    QString html = QString(QLatin1String(file.readAll()))
                        .arg(title)
                        .arg(errorString)
                        .arg(reply->url().toString());

    QBuffer imageBuffer;
    imageBuffer.open(QBuffer::ReadWrite);
    QIcon icon = view()->style()->standardIcon(QStyle::SP_MessageBoxWarning, 0, view());
    QPixmap pixmap = icon.pixmap(QSize(32,32));
    if (pixmap.save(&imageBuffer, "PNG")) {
        html.replace(QLatin1String("IMAGE_BINARY_DATA_HERE"),
                     QString(QLatin1String(imageBuffer.buffer().toBase64())));
    }

    QList<QWebFrame*> frames;
    frames.append(mainFrame());
    while (!frames.isEmpty()) {
        QWebFrame *frame = frames.takeFirst();
        if (frame->url() == reply->url()) {
            frame->setHtml(html, reply->url());
            return;
        }
        QList<QWebFrame *> children = frame->childFrames();
        foreach(QWebFrame *frame, children)
            frames.append(frame);
    }
    if (m_loadingUrl == reply->url()) {
        mainFrame()->setHtml(html, reply->url());
    }
    //cout<<"reply~~~~~~~~~~~~~~~~~~~~~~~"<<reply->url().toString().toUtf8().data()<<endl;
}


WebView::WebView(QWidget* parent)
    : QWebView(parent)
    , m_progress(0)
    , m_page(new WebPage(this))
    , test(1)
{
/////////////////{H}
timer2 = new QTimer(this);
QObject::connect(timer2, SIGNAL(timeout()), this, SLOT(write_logfile()));

//hit_timer = new QElapsedTimer(this);
////////////////
    setPage(m_page);
    connect(page(), SIGNAL(statusBarMessage(QString)),
            SLOT(setStatusBarText(QString)));
    connect(this, SIGNAL(loadProgress(int)),
            this, SLOT(setProgress(int)));
    connect(this, SIGNAL(loadFinished(bool)),
            this, SLOT(loadFinished()));
    connect(page(), SIGNAL(loadingUrl(QUrl)),
            this, SIGNAL(urlChanged(QUrl)));
    connect(page(), SIGNAL(downloadRequested(QNetworkRequest)),
            this, SLOT(downloadRequested(QNetworkRequest)));
    connect(page(), SIGNAL(linkClicked(QUrl)),
          this, SLOT(linkClicked(QUrl)));
    page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks); 
    page()->setForwardUnsupportedContent(true);
}

void WebView::contextMenuEvent(QContextMenuEvent *event)
{
    QWebHitTestResult r = page()->mainFrame()->hitTestContent(event->pos());

    if (!r.linkUrl().isEmpty()) {
        QMenu menu(this);
        menu.addAction(pageAction(QWebPage::OpenLinkInNewWindow));
        menu.addAction(tr("Open in New Tab"), this, SLOT(openLinkInNewTab()));
        menu.addSeparator();
        menu.addAction(pageAction(QWebPage::DownloadLinkToDisk));
        // Add link to bookmarks...
        menu.addSeparator();
        menu.addAction(pageAction(QWebPage::CopyLinkToClipboard));
        if (page()->settings()->testAttribute(QWebSettings::DeveloperExtrasEnabled))
            menu.addAction(pageAction(QWebPage::InspectElement));
        menu.exec(mapToGlobal(event->pos()));
        return;
    }
    QWebView::contextMenuEvent(event);
}

void WebView::wheelEvent(QWheelEvent *event)
{
    if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
        int numDegrees = event->delta() / 8;
        int numSteps = numDegrees / 15;
        setTextSizeMultiplier(textSizeMultiplier() + numSteps * 0.1);
        event->accept();
        return;
    }
    QWebView::wheelEvent(event);
}

void WebView::openLinkInNewTab()
{
    m_page->m_openInNewTab = true;
    pageAction(QWebPage::OpenLinkInNewWindow)->trigger();
}

void WebView::setProgress(int progress)
{
    m_progress = progress;
}

//{H}
void WebView::cal_cout()
{
	if(preloadTotalTime > 3000000000) {
		preloadTotalTime = preloadTotalTime - 3000000000;
		cout_preloadTotalTime = cout_preloadTotalTime + 1;
	}

	if(totalTime > 3000000000) {
		totalTime = totalTime - 3000000000;
		cout_totalTime = cout_totalTime + 1;
	}
}

void WebView::loadFinished()
{
////////////////////////////////////intelligent browser start
 

    if (100 != m_progress) {
       qWarning() << "Received finished signal while progress is still:" << progress()
                   << "Url:" << url();
//cout<< "reload()\n";
	//QTimer::singleShot(5000, this, SLOT(reload()));
    }
	//m_progress = 0;
    /*if (timer->isActive()) {
cout<<"reload()\n";
	timer->stop();	//sslab_alive
	QTimer::singleShot(8000, this, SLOT(reload()));
    }*/
   TabWidget* temp_tabWidget = m_page->mainWindow()->tabWidget();   
    int tab_index;     
 ////////////////////////////////////intelligent browser end  
    tab_index = temp_tabWidget->webViewIndex(this);
my_queue* new_queue=new my_queue;
////////////////////////////////////intelligent browser start
    if( tab_index ==5)   
    {
       
	if( title()!= QString())
	{
	    global_msecs = load_timer.elapsed();
	    long nsecs = load_timer.nsecsElapsed();

            cout << " ---------------------------------------------------------------\n" <<endl;
	    printf(" %s \n Original loading time: %ld msecs %ld nsecs \n\n",url().toString().toUtf8().data(),global_msecs , nsecs);
	    cout << " ---------------------------------------------------------------\n" <<endl;
	    //{H}
  	    BrowserApplication::count = BrowserApplication::count+1;
//// edit_Graph start
    BrowserApplication::gethead()->set_temp_node(BrowserApplication::gethead()->count( BrowserApplication::gethead()->get_temp_node(), H_url, global_msecs));

//{H} for compare with times
		/*abc++;
		if((abc%20) == 0) {
			write_bufinfo();
		}*/
//cout << "URL count : " <<abc<<endl;
/*if(global_msecs >=0 && global_msecs <= 20000){
cout << "msecs " <<global_msecs<<endl;
		preloadTotalTime += global_msecs;
		totalTime += global_msecs;
		cal_cout();
		//global_msecs=0;
}*/

        new_queue->set_node(BrowserApplication::gethead()->get_temp_node());

        BrowserApplication::gethead()->G_queue->Enqueue(BrowserApplication::gethead()->G_queue,new_queue); 
//// edit_Graph  end
		/*if(BrowserApplication::simulate_SW == 1) {
			go_emit();
		}
		cout << "PRE_write\n";
		if(BrowserApplication::simulate_SW == 1) {
			QTimer::singleShot(5000, this, SLOT(go_emit()));
		}
		timer2->start(5000);*/
		/*else if(BrowserApplication::extract_SW == 1) {
cout<< " BrowserApplication::extract_SW == 1\n";
			QTimer::singleShot(5000, this, SLOT(write_logfile()));
		}*/
		BrowserApplication::gethead()->get_temp_node()->fill_edge();
		temp_tabWidget = m_page->mainWindow()->tabWidget();  
		temp_tabWidget->load_hiddenTab(BrowserApplication::gethead(),BrowserApplication::gethead()->get_temp_node()->url); 
	}
   }

   check_flag=0;
    
////////////////////////////////////intelligent browser end
	m_progress = 0;
}

void WebView::loadUrl(const QUrl &url)
{
    m_initialUrl = url;
    load(url);
}

QString WebView::lastStatusBarText() const
{
    return m_statusBarText;
}

QUrl WebView::url() const
{
    QUrl url = QWebView::url();
    if (!url.isEmpty())
    {
      //`  printf("webview.cpp --url() url :%s\n",url.toString().toUtf8().data());
        return url;
    }
    return m_initialUrl;
}

void WebView::mousePressEvent(QMouseEvent *event)
{
    m_page->m_pressedButtons = event->buttons();
    m_page->m_keyboardModifiers = event->modifiers();
    QWebView::mousePressEvent(event);
}

void WebView::mouseReleaseEvent(QMouseEvent *event)
{
    QWebView::mouseReleaseEvent(event);
#ifndef QT_NO_CLIPBOARD
    if (!event->isAccepted() && (m_page->m_pressedButtons & Qt::MidButton)) {
        QUrl url(QApplication::clipboard()->text(QClipboard::Selection));
        if (!url.isEmpty() && url.isValid() && !url.scheme().isEmpty()) {
            setUrl(url);
        }
    }
#endif
}

void WebView::setStatusBarText(const QString &string)
{
    m_statusBarText = string;
}

void WebView::downloadRequested(const QNetworkRequest &request)
{
    BrowserApplication::downloadManager()->download(request);
}

////////////////////////////////////intelligent browser start
void WebView::my_linkclicked(QUrl url) //다른 클래스에서 linkClicked을 부르기 위해 사용
{
    linkClicked(url); 
}

/* 
 bool WebView::linkClicked(QUrl url)
 1. url 요청이 들어올 때  실행 
 2. hit시 탭교환
 3. 그래프를 갱신하고 히든탭에 예측 페이지 로딩
*/
bool WebView::linkClicked(QUrl url) 
{
	load_timer.start();	//{H}
    hit_timer.start();
    check_flag=1;

    int num;
	int view_index;

	int modSet = 0; //{H} modSet
 
    time_t cur_time;
    struct tm * cur_tm;
    cur_time = time(NULL);
    cur_tm= localtime(&cur_time);

    char s_edit_url[2000]={0,};

    my_queue* new_queue=new my_queue;
    ofstream fin;

    TabWidget* temp_tabWidget = m_page->mainWindow()->tabWidget();	
    strcpy(s_edit_url,url.toString().toUtf8().data());
    strcpy(s_edit_url,edit_url(s_edit_url));

	for(int i=0; i<5 ;i++) //tab change
	{
if(BrowserApplication::extract_SW == 0) {	//{H} mod select
		if(!strcmp(temp_tabWidget->get_preurl(i).toString().toUtf8().data(),s_edit_url))	//{HONG} when the request Node is the same with the URL Node
        	{
            	temp_tabWidget->tabBar()->moveTab(i,5);
		temp_tabWidget->tabBar()->moveTab(4,i); //{H} not need
//		temp_tabWidget->tabBar()->tabHide(i, i);
		temp_tabWidget->tabBar()->tabShow(5);
		
		temp_tabWidget->setCurrentIndex(5);

	    	long msecs2 = hit_timer.elapsed();
	    	long nsecs2 = hit_timer.nsecsElapsed();
		//timer.start();
		cout<<"|=={Hit}==|"<<endl;
		cout << " ---------------------------------------------------------------\n" <<endl;
	    	printf(" %s \n Tab Change time : %ld msecs %ld nsecs \n\n",s_edit_url,msecs2 , nsecs2);
		cout << " ---------------------------------------------------------------\n" <<endl;
		//{H}
		BrowserApplication::count = BrowserApplication::count+1;
		BrowserApplication::hit_count = BrowserApplication::hit_count+1;
//// edit_Graph start
    BrowserApplication::gethead()->set_temp_node(BrowserApplication::gethead()->count( BrowserApplication::gethead()->get_temp_node(),s_edit_url, msecs2));

//{H}
		preloadTotalTime += msecs2;
		//cal_cout();
		abc++;
		if(BrowserApplication::gethead()->get_temp_node()->msecs <= 20000 && BrowserApplication::gethead()->get_temp_node()->msecs > 0)
			totalTime += BrowserApplication::gethead()->get_temp_node()->msecs;
		else
			totalTime += msecs2;
		//if((abc%50) == 0) {
			//write_bufinfo();
		//}
cout << "URL count : " <<abc<<endl;
cout << "Tab change time using preloading : " << msecs2 << endl;
cout << "Loading time without preloading : " << BrowserApplication::gethead()->get_temp_node()->msecs << endl << endl;

        new_queue->set_node(BrowserApplication::gethead()->get_temp_node());

        BrowserApplication::gethead()->G_queue->Enqueue(BrowserApplication::gethead()->G_queue,new_queue); 
//// edit_Graph  end

	    	temp_tabWidget->load_hiddenTab(BrowserApplication::gethead(),s_edit_url);
		temp_tabWidget->setCurrentIndex(5);

/////////////////////////
// {H} SIMULATE
/////////////////////////
cout << "BrowserApplication::simulate_SW : " << BrowserApplication::simulate_SW <<endl;
			/*if(BrowserApplication::simulate_SW == 1) {
				//QTimer *timer = new QTimer(this);
				//timer->start(5000);
				QTimer::singleShot(5000, this, SLOT(go_emit()));
				//go_emit();
			}*/
		return 0;
	        }
}
	}

	//edit_url(url.toString().toUtf8().data());
	//temp_tabWidget->setCurrentIndex(5);
	//load(url);
	strcpy(H_url, s_edit_url);
	load(QUrl(s_edit_url));
        temp_tabWidget->load_hiddenTab(BrowserApplication::gethead(),s_edit_url);

    return 0;
}

//{H}
void WebView::go_emit()
{
//timer.start();
	char url_data[1200] = { 0, };
	char temp_token[1200]={0,};

	if(BrowserApplication::sfin.fail() == false){
		while(!BrowserApplication::sfin.eof())
		{
			BrowserApplication::sfin.getline(url_data,1200);
			if (strlen(url_data) == 0) continue;
			
			strcpy(temp_token,strtok(url_data,"* \n"));
	
			if (strcmp(temp_token, "url") == 0)
			{
//sleep(1);
				strcpy(temp_token, strtok(NULL, " "));
cout <<"in WebView::go_emit() : " << temp_token << endl;
				emit linkClicked(QUrl(temp_token));
				memset(temp_token, 0, 1200);
				return;
			}

		}
		if(BrowserApplication::sfin.eof()) {
			cout<<"ALL_ hit / total : "<< BrowserApplication::hit_count << " / " << BrowserApplication::count << endl; 
			cout << "---------------------------------------------------------\n";
			cout << "END_ Estimated total loading time without preloading : " << totalTime << ", cout : " << cout_totalTime << endl;
			cout << "END_ Total loading time using preloading : " << preloadTotalTime << ", cout : " << cout_preloadTotalTime << endl;
			cout << "---------------------------------------------------------\n\n";

			exit(0);
		}
	}

	return;
}

void WebView::write_bufinfo()
{
	ofstream bout;
	cr++;
	bout.open("history/statics5.txt",ios::app);
	
	cout << "hit / total : "<< BrowserApplication::hit_count << " / " << BrowserApplication::count << " => " << BrowserApplication::hit_count/BrowserApplication::count << endl;
	cout << "Estimated total loading time without preloading : " << totalTime << ", cout : " << cout_totalTime << endl;
	cout << "Total loading time using preloading : " << preloadTotalTime << ", cout : " << cout_preloadTotalTime << endl;
	cout << "---------------------------------------------------------\n\n";

	bout << "hit / total : "<< BrowserApplication::hit_count << " / " << BrowserApplication::count << " => " << BrowserApplication::hit_count/BrowserApplication::count << endl;
	bout << "Estimated total loading time without preloading : " << totalTime << ", cout : " << cout_totalTime << endl;
	bout << "Total loading time using preloading : " << preloadTotalTime << ", cout : " << cout_preloadTotalTime << endl;
	bout << "---------------------------------------------------------\n\n";
	
	bout.close();

	return;
}

void WebView::write_logfile()
{
	//char url_data[1200] = { 0, };
	ofstream fout;
	cr++;
	fout.open("history/historglog5.txt",ios::app);
if(global_msecs >= 0 && global_msecs <= 50000) {
	if(cr==1)
		fout<<"url* " << "http://www.naver.com/" <<endl;
	else if(global_msecs > 0)
		fout<<"url* " << url_data <<endl;
cout<<"WebView::write_logfile() url* " << url_data <<endl;
cout<<"url* LENGTH " << strlen(url_data) <<endl;
	if(global_msecs > 0)
		fout<<"loading_time* " << global_msecs << endl; //{H} 
	fout<<"\n" << endl; //{H} 
}
	//fout<< "ctime* " << ctime(&temp_queue->cur_time)<<endl;
		//fout<<"loading_time* " << history->msecs << endl; //{H} 
//cout << "time(&temp_queue->cur_time) : " <<time(&temp_queue->cur_time) << endl;

	fout.close();
	
////////////////////////////////////
	if(BrowserApplication::sfin.fail() == false){
		while(!BrowserApplication::sfin.eof())
		{
			memset(&url_data[0], 0, 1200);
			BrowserApplication::sfin.getline(url_data,1200);
			if(strlen(url_data) == 0) continue;
			if(strlen(url_data) > 1199) continue;			
			if (strstr(url_data, "URL") != 0)
			{	
				if(strstr(url_data, "Length") == 0) {
					if(strlen(url_data) < 1170) {
						for(int i=0; i< strlen(url_data); i++) {
							if(url_data[i] == ' ') {
								if(url_data[i+1] == ':' && url_data[i+2] == ' ') {
									memmove(&url_data[0], &url_data[i+3], strlen(url_data)-(i+3));
									memset(&url_data[strlen(url_data)-(i+3)], 0, strlen(url_data)-(strlen(url_data)-(i+3)));
								} 			
							}
						}
						load_timer.start();	//{H}
						emit linkClicked(QUrl(url_data));
						break;
					}
				}
			}	
		}
	}

	return;
}
/*
 char* WebView::edit_url(char* url)
 1. naver url 수정
 2. bool WebView::linkClicked(QUrl url)에서 사용
*/

char* WebView::edit_url(char* url) 
{
	char* real_url=NULL;
	int idx = 0;

	string s;

	//{H}/////////
	char* tempStr=NULL;
	//////////////
	if (strstr(url, "%") != 0)
	{	
		for(int i=0; i< strlen(url); i++) {
			if(url[i] == '%') {
				if(url[i+1] == '0' && url[i+2] == 'D') {
					memset(&url[i], 0, 3);
				}
				if(url[i+1] >= 48 && url[i+1] <= 55) {
					if(url[i+2] >= 48 && url[i+2] <= 55) {
						url[i] = ((url[i+1]-48)*16) + (url[i+2]-48);
						memmove(&url[i+1], &url[i+3], strlen(url)-(i+3));			
						memset(&url[strlen(url)-1-1], 0, 2);	
					}
					else if(url[i+2] >= 65 && url[i+2] <= 70) {
						url[i] = ((url[i+1]-48)*16) + (url[i+2]-55);
						memmove(&url[i+1], &url[i+3], strlen(url)-(i+3));
			
						memset(&url[strlen(url)-1-1], 0, 2);
					}
				}
				else if(url[i+1] >= 65 && url[i+1] <= 70) {
					url[i] = url[i+1]-55;
					
					memmove(&url[i+1], &url[i+2], strlen(url)-(i+2));
					memset(&url[strlen(url)-1], 0, 1);
				} 
			}
		}
	}

	if (strstr(url, "&u=") != 0)
	{
		real_url=back_strtok(url);
		s = real_url + 2;
		while (s.find("%3F")!=-1)
		{	
			idx = s.find("%3F");
			s.replace(idx, 3, "?");
		}
		while (s.find("%3A")!=-1)
		{
			idx = s.find("%3A");
			s.replace(idx,3,":");
		}
		while (s.find("%2F") !=-1)
		{
			idx = s.find("%2F");
			s.replace(idx, 3, "/");
		}
		while (s.find("%3D")!=-1)
		{
			idx = s.find("%3D");
			s.replace(idx, 3, "=");
		}
		while (s.find("%26") !=-1)
		{
			idx = s.find("%26");
			s.replace(idx, 3, "&");
		}
		return (char*)s.c_str();
	}
	else if (strstr(url, "nil_profile=mini&nil_src=") != 0)
	{
		tempStr = back_strtok2(url);
		memset(tempStr, 0, strlen(tempStr));
	}

	return url;
}

char* WebView::back_strtok(char *str)						// back strtoken
{
	int i;
	int length = 0;
	length = strlen(str);
	for (i = length; i >= 0; i--){
		if (str[i] == '&' && str[i+1] == 'u' && str[i+2] == '='){
			str[i] = 0;					// if '/', save NULL
			break;
		}
	}
	return &str[i + 1];							// cut string
}

char* WebView::back_strtok2(char *str)						// back strtoken
{
	int i;
	int length = 0;
	length = strlen(str);
	for (i = length; i >= 0; i--){
		if (str[i] == '/' && str[i+1] == '?'){
			//str[i] = 0;					// if '/', save NULL
			break;
		}
	}
	//cout << &str[i] <<endl;
	return &str[i];							// cut string
}
////////////////////////////////////intelligent browser end
