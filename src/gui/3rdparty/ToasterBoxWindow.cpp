
#include <wx/wx.h>
// #include "wx/wxprec.h"
#include <wx/dialog.h>
#include <wx/utils.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/timer.h>
#include <wx/dcclient.h>
#include <wx/region.h>
#include "ToasterBoxWindow.h"

long ToasterBoxWindow::count = 0;

ToasterBoxWindow::ToasterBoxWindow(wxWindow* parent, wxTimer *_parent2):
  wxFrame(parent, 0, "window",
  wxPoint(0,0), wxDefaultSize, wxNO_BORDER|wxSTAY_ON_TOP|wxFRAME_NO_TASKBAR)
{ 
  startTime = wxGetLocalTime();
  parent2 = _parent2;
  sleepTime = 10;
  step = 4;
  pauseTime = 1700;
  textColor = *wxWHITE;
  popupText = "Change Me!";
  // the size we want the dialog to be
  wxSize dialogSize(150, 170);
  count++;


  bottomRight = wxPoint(wxGetDisplaySize().GetWidth(),
    wxGetDisplaySize().GetHeight());

  SetSize(bottomRight.x, bottomRight.y,
    dialogSize.GetWidth(), dialogSize.GetHeight());
}

void ToasterBoxWindow::SetPopupBitmap(wxString bitmapFile)
{    
  if (!bitmapFile.IsEmpty())
  {
    wxBitmap bm = wxBitmap();
    bm.LoadFile(bitmapFile, wxBITMAP_TYPE_BMP);
    if (bm.Ok())
      sbm.Create(this, 0, bm, wxPoint(0,0));    
  }
  // wxBitmap bm2 = wxBitmap();
  // bm2.LoadFile("back2.bmp", wxBITMAP_TYPE_BMP);
  // wxRegion reg(bm2);
  // wxLogDebug("%i", SetShape(*reg));
}


void ToasterBoxWindow::SetPopupSize(int x, int y)
{
  SetSize(bottomRight.x, bottomRight.y,
    x, y);  
}

void ToasterBoxWindow::SetPopupPosition(int x, int y)
{  
  dialogTop = wxPoint(x,y);
  bottomRight = wxPoint(dialogTop.x + GetSize().GetWidth(),
    dialogTop.y + GetSize().GetHeight());
}


// _shrink indicates if we should to transformations on
// the text to make it "pretty"
void ToasterBoxWindow::SetPopupText(wxString _text, bool _shrink)
{
  popupText = _text; 
  shrink = _shrink;
}

// activate the frame, raise it and set the timer
bool ToasterBoxWindow::Play()
{
  // do some checks to make sure this window is valid
  if (bottomRight.x < 1 || bottomRight.y < 1)
    return false;
  if (GetSize().GetWidth() < 50 || GetSize().GetWidth() < 50)
  {
    // toasterbox launches into a endless loop for some reason
    // when you try to make the window too small.
    return false;
  }
  ScrollUp();
  Start(pauseTime, true);
  return true;
}

// return to the user if their text all fit or not
bool ToasterBoxWindow::DoesTextFit()
{
  wxCoord w, h;
  wxClientDC dc(this);  
  dc.GetTextExtent(popupText, &w, &h);
  if (w > GetSize().GetWidth() || h > GetSize().GetHeight())
    return false;
  return true;
}

void ToasterBoxWindow::SetPopupBackgroundColor(int r, int g, int b)
{
  if (wxColor(r,g,b).Ok())    
    SetBackgroundColour(wxColor(r,g,b));
  else
    wxLogDebug("invalid color");
}

void ToasterBoxWindow::SetPopupTextColor(int r, int g, int b)
{
  if (wxColor(r,g,b).Ok())    
    textColor = wxColor(r,g,b);
}

// private functions
void ToasterBoxWindow::ScrollUp()
{
  wxLogDebug("Raising");

  Show();  

  // walk the Y value up in a raise motion
  int windowSize = 0;
  for (int i = bottomRight.y; i > dialogTop.y; i -= step)
  {
    if (i < dialogTop.y)
      i = dialogTop.y;
    windowSize += step;
    SetSize(dialogTop.x, i, GetSize().GetWidth(), 
      windowSize);    
    // Update();
      wxUsleep(sleepTime);        
  }
  Update();
  DrawText();
  Update();
}

void ToasterBoxWindow::ScrollDown()
{
  wxLogDebug("Lowering");

  // walk down the Y value
  int windowSize = GetSize().GetHeight();
  for (int i = dialogTop.y; i < bottomRight.y; i += step)
  {
    if (i > bottomRight.y)
      i = bottomRight.y;
    windowSize -= step;
    SetSize(dialogTop.x, i, GetSize().GetWidth(), 
      windowSize);    
    // Update();
      wxUsleep(sleepTime);   

  }
  Hide();
  if (parent2)
    parent2->Notify();
  else
    wxLogDebug("not valid parent");
}

void ToasterBoxWindow::DrawText()
{
  // width and height of text
  wxCoord w = 0, h = 0;
  // where we will set the text
  wxCoord x = 5, y = 0;
  // border from sides and top to text (in pixels)
  int border = 7;
  // how much space between text lines
  int textPadding = 4;
  // how much can we have on a line?
  int textColumns = 0;
  // how many lines can we fit vertically
  float textLines = 1;
  // the popupText after our transformations (if we do any)
  wxString pText = GetPopupText();

  wxClientDC dc(this);
  dc.GetTextExtent(pText, &w, &h);
  
  // shrink the text to fit in the popup box
  if (shrink)
  {    
    while((w +(border * 2))> GetSize().GetWidth())
    {
      pText = pText.Left(pText.Length()-1);    
      dc.GetTextExtent(pText, &w, &h);
    }    
    textColumns = pText.Length();
    // figure out how many lines of text we have    
    textLines = (GetPopupText().Length() / textColumns) +.9;
  }

  // center the text
  if (w < GetSize().GetWidth())      
    x = (GetSize().GetWidth() - w) / 2;
  if (h < GetSize().GetHeight())
    y = (GetSize().GetHeight() - h) / 2;
  
  dc.SetTextForeground(textColor);

  // if we have multiple lines
  if (textLines > 1)
  {
    // how many lines we can fit in the height available
    float howManyLinesFit = GetSize().GetHeight() / (h+textPadding);
    if (textLines < howManyLinesFit)
      howManyLinesFit = textLines;
    y = (GetSize().GetHeight() - ((h+textPadding) * howManyLinesFit)) / 2;    
    for (int i = 0; i < howManyLinesFit; i++)
    {
      dc.DrawText(GetPopupText().Mid((i*textColumns), textColumns), x,y);
      // move the text down a line
      y += h+textPadding;
    }
  }
  else      
    dc.DrawText(pText, x,y);
  
}

void ToasterBoxWindow::Notify()
{
  wxLogDebug("Been up for: %i", wxGetLocalTime() - startTime);
  ScrollDown();
}

void ToasterBoxWindow::PrintInfo()
{
  wxLogDebug("brX:%i brY:%i szW:%i szH:%i",
    bottomRight.x, bottomRight.y, GetSize().GetWidth(),
    GetSize().GetHeight());
}