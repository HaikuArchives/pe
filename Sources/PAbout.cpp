/*	$Id$

	Copyright 1996, 1997, 1998, 2002
	        Hekkelman Programmatuur B.V.  All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:
	1. Redistributions of source code must retain the above copyright notice,
	   this list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright notice,
	   this list of conditions and the following disclaimer in the documentation
	   and/or other materials provided with the distribution.
	3. All advertising materials mentioning features or use of this software
	   must display the following acknowledgement:

	    This product includes software developed by Hekkelman Programmatuur B.V.

	4. The name of Hekkelman Programmatuur B.V. may not be used to endorse or
	   promote products derived from this software without specific prior
	   written permission.

	THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
	INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
	FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
	AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
	EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
	OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
	WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
	OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
	ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Created: 09/15/97 23:11:37
*/

#include "pe.h"
#include "PAbout.h"
#include "HAppResFile.h"
#include "HError.h"
#include "ResourcesMisc.h"
#if STANDALONE
#else
#	include "PApp.h"
#endif

const char kAboutText[] =
		"Pe 2.4.6 Open Source Version\n\n"
		"a high performance editor\n"
		"created by Maarten Hekkelman.\n"
		"Hekkelman Programmatuur B.V.\n"
		"Copyright © 1996, 1997, 1998, 2002.\n\n"
		"-\n\n"
		"This is the Open Source version of Pe,\n"
		"copyright © 2003-2024, Team Pe,\n"
		"compiled on:\n%s\n\n"
		"Pe is now hosted at Github:\n"
		"https://github.com/HaikuArchives/Pe\n\n"
		"You can reach Team Pe\n"
		"via Pe's mailing-list:\n"
		"pedevel@freelists.org\n\n"
		"-\n\n"
		"Please send suggestions and/or bugreports\n"
		"to the list or use the github tools.\n\n"
		"-\n\n"
		"Thanks to all contributors (in alphabetical order):\n\n"
		"Andrew Bachmann\n"
		"Axel Dörfler\n"
		"Chris Roberts\n"
		"Christian Packmann\n"
		"François Revol\n"
		"Guy Haviv\n"
		"Ingo Weinhold\n"
		"Ithamar R. Adema\n"
		"Keith McAfee\n"
		"Oliver Tappe\n"
		"Oscar Lesta\n"
		"Rainer Riedl\n"
		"Stephan Aßmus\n\n\n"
		"...and thanks to everyone we forgot, too!\n\n";

#ifdef __HAIKU__
const char kAboutText2[] = "Haiku Rules!";
#else
const char kAboutText2[] = "BeOS Rules!";
#endif

#if STANDALONE
const rgb_color
	gKeywordColor = { 0, 0, 255, 0 },
	gCommentColor = { 255, 0, 0, 0 };
#endif

const long kPEOffsetH = 95;
const long kPEOffsetV = 60;
const long kAboutTextHeight = 100;

const bigtime_t kDelay = 20000;

const unsigned long msg_Animate = 'anim';

static const char *Now()
{
#if STANDALONE
	return "now";
#else
	static const char *now = (char *)HResources::GetResource(rtyp_Time,
		rid_Time_Compiled);
	return now;
#endif
} /* Now */

// #pragma mark - PAboutBox

PAboutBox::PAboutBox()
	: BWindow(BRect(100, 100, 200, 200), "aboutbox", B_BORDERED_WINDOW, 0)
{
	fAboutText = (char *)malloc(strlen(kAboutText) + strlen(Now()) + 1);
#if STANDALONE
#else
	FailNil(fAboutText);
#endif
	sprintf(fAboutText, kAboutText, Now());

	BRect textRect;
	DetermineBounds(textRect);
	ResizeAndCenter(textRect);

	// Setup off-screen bitmap

	fOffscreen = new BBitmap(Bounds(), B_RGB_32_BIT, true);

	fPFont.SetFamilyAndStyle("Baskerville", "Roman");
	fPFont.SetSize(60);
	fPFont.SetSpacing(B_CHAR_SPACING);
	fEFont = fPFont;

	fPFont.SetShear(45);

	if (fOffscreen->Lock())
	{
		fOffscreen->AddChild(fOffView = new BView(Bounds(), "view", 0,
			B_WILL_DRAW));
		fOffView->SetFont(&fPFont);
		fOffscreen->Unlock();
	}

	// Render text into another off-screen bitmap

	fOffText = new BBitmap(textRect, B_RGB_32_BIT, true);
	BView *textView = new BView(textRect, "text", 0, B_WILL_DRAW);
	fOffText->AddChild(textView);

	if (fOffText->Lock())
	{
		DrawText(fAboutText, textView);
		textView->Sync();
		fOffText->Unlock();
	}

	AddChild(fView = new PAboutView(Bounds(), fOffscreen, fOffText));

	fTextViewD.Set(10, 100, 10 + textRect.Width(), Bounds().Height());
	fTextViewS = fTextViewD;
	fTextViewS.OffsetTo(0, 0);

	fState1 = 0;
	fState2 = 0;
	fVelocity = 10;
	fNextFrame = 0;

	this->PostMessage(msg_Animate);

	Show();
} /* PAboutBox::PAboutBox */

PAboutBox::~PAboutBox()
{
	delete fOffscreen;
	free(fAboutText);

	delete fOffText;

#if STANDALONE
	be_app->PostMessage(B_QUIT_REQUESTED);
#endif
} /* PAboutBox::~PAboutBox */

void PAboutBox::MessageReceived(BMessage *msg)
{
	if (msg->what == msg_Animate)
		Animate();
	else
		BWindow::MessageReceived(msg);
} /* PAboutBox::MessageReceived */

void PAboutBox::Animate()
{
	BRect b(Bounds());

	bigtime_t now = system_time();
	if (fNextFrame > now)
	{
		snooze(fNextFrame - now);
		fNextFrame += kDelay;
	}
	else
		fNextFrame = now + kDelay;

	if (fOffscreen->Lock())
	{
		fOffView->FillRect(b, B_SOLID_LOW);
		fOffView->DrawBitmap(fOffText, fTextViewS, fTextViewD);

		int i;
		uchar wipe[8] = { 0xEE, 0xCC, 0xAA, 0x88, 0x66, 0x44, 0x22, 0x00 };

		fOffView->SetDrawingMode(B_OP_ADD);

		for (i = 0; i < 7; i++)
		{
			fOffView->SetHighColor(wipe[i], wipe[i], wipe[i]);

			BPoint pl = fTextViewD.LeftTop(), pr = fTextViewD.RightTop();
			pl.y += i; pr.y += i;

			fOffView->StrokeLine(pl, pr);

			pl = fTextViewD.LeftBottom(), pr = fTextViewD.RightBottom();
			pl.y -= i; pr.y -= i;

			fOffView->StrokeLine(pl, pr);
		}

		fOffView->SetDrawingMode(B_OP_COPY);

		if ((++fTextStep % 2) == 0)
		{
			fTextViewS.OffsetBy(0, 1);
			if (fTextViewS.bottom > fOffText->Bounds().bottom)
			{
				fTextViewS.OffsetTo(0, 0);
			}
		}

		switch (fState1)
		{
			case 0:
			{
//				fOffView->FillRect(b, B_SOLID_LOW);
//				fOffView->SetHighColor(gColor[kColorKeyword1]);
				fOffView->SetHighColor(0x39, 0x74, 0x79, 255);

				BPoint p;
				p.y = kPEOffsetV;
				p.x = b.left + fState2;

				fState2 += (fVelocity *= 0.95);

				fOffView->DrawString("P", p);

				if (fState2 >= kPEOffsetH)
				{
					fState1++;
					fState2 = 45;
					fVelocity = 7.5;
				}
				break;
			}

			case 1:
			{
//				fOffView->FillRect(b, B_SOLID_LOW);
				fOffView->SetHighColor(0x39, 0x74, 0x79, 255);
				BPoint p;
				p.y = kPEOffsetV;
				p.x = b.left + kPEOffsetH;

				fPFont.SetShear(fState2);
				fOffView->SetFont(&fPFont);

				fOffView->DrawString("P", p);

				if (fState2 <= 90)
					fState2 += (fVelocity *= 1.05);
				else if (fState2 < 135)
					fState2 += (fVelocity *= 0.95);
				else
					fState1++;
				break;
			}

			case 2:
			{
//				fOffView->FillRect(b, B_SOLID_LOW);
				fOffView->SetHighColor(0x39, 0x74, 0x79, 255);

				BPoint p;
				p.y = kPEOffsetV;
				p.x = b.left + kPEOffsetH;

				fPFont.SetShear(fState2);
				fOffView->SetFont(&fPFont);

				fOffView->DrawString("P", p);

				if (fState2 > 90)
					fState2 -= (fVelocity *= 0.95);
				else
				{
					fPFont.SetShear(90);
					fState1++;
					fState2 = b.right;
					fPStop = kPEOffsetH + fPFont.StringWidth("P");
					fVelocity = fState2 / 25;
				}
				break;
			}

			case 3:
			{
//				fOffView->FillRect(b, B_SOLID_LOW);
				fOffView->SetHighColor(0x39, 0x74, 0x79, 255);

				BPoint p;
				p.y = kPEOffsetV;
				p.x = b.left + kPEOffsetH;

				fOffView->SetFont(&fPFont);
				fOffView->DrawString("P", p);

				p.x = fState2;

				fState2 -= (fVelocity *= 0.95);

				fOffView->SetHighColor(0xa1, 0x64, 0x0e, 255);
				fEFont.SetShear(90);
				fOffView->SetFont(&fEFont);

				fOffView->DrawString("e", p);

				if (fState2 <= fPStop)
				{
					fState1 += 2;
//					fState2 = 135;
					fState2 = 90;
					fVelocity = 7.5;
				}
				break;
			}

			case 4:
			{
//				fOffView->FillRect(b, B_SOLID_LOW);
				fOffView->SetHighColor(0x39, 0x74, 0x79, 255);

				BPoint p;
				p.y = kPEOffsetV;
				p.x = b.left + kPEOffsetH;

				fOffView->SetFont(&fPFont);
				fOffView->DrawString("P", p);

				p.x = fPStop;

				fOffView->SetHighColor(0xa1, 0x64, 0x0e, 255);
				fEFont.SetShear(fState2);
				fOffView->SetFont(&fEFont);

				fOffView->DrawString("e", p);

				if (fState2 > 90)
					fState2 -= (fVelocity *= 1.05);
				else
					fState1++;
				break;
			}

			case 5:
			{
//				fOffView->FillRect(b, B_SOLID_LOW);
				fOffView->SetHighColor(0x39, 0x74, 0x79, 255);

				BPoint p;
				p.y = kPEOffsetV;
				p.x = b.left + kPEOffsetH;

				fPFont.SetShear(fState2);
				fOffView->SetFont(&fPFont);

				fOffView->DrawString("P", p);
				fOffView->SetHighColor(0xa1, 0x64, 0x0e, 255);
				fOffView->DrawString("e");

				if (fState2 <= 45)
					fState1++;
				else
					fState2 -= (fVelocity *= 0.95);
				break;
			}

			case 6:
			{
//				fOffView->FillRect(b, B_SOLID_LOW);
				fOffView->SetHighColor(0x39, 0x74, 0x79, 255);

				BPoint p;
				p.y = kPEOffsetV;
				p.x = b.left + kPEOffsetH;

				fPFont.SetShear(fState2);
				fOffView->SetFont(&fPFont);

				fOffView->DrawString("P", p);
				fOffView->SetHighColor(0xa1, 0x64, 0x0e, 255);
				fOffView->DrawString("e");

				if (fState2 < 90)
					fState2 += (fVelocity *= 0.95);
				else
				{
					fState1++;
//					fPFont.SetShear(90);
//					fState2 = b.right;
//					fPStop = - be_plain_font->StringWidth(fAboutText);
//				}
//				break;
//			}
//
//			case 7:
//			{
////				fOffView->FillRect(b, B_SOLID_LOW);
//
//				BPoint p;
//				p.y = kPEOffsetV;
//				p.x = b.left + kPEOffsetH;
//
//				fOffView->SetFont(&fPFont);
//
//				fOffView->SetHighColor(0x39, 0x74, 0x79, 255);
//				fOffView->DrawString("P", p);
//				fOffView->SetHighColor(0xa1, 0x64, 0x0e, 255);
//				fOffView->DrawString("e");
//
//				p.y = b.bottom - 4;
//				p.x = fState2;
//
//				fOffView->SetFont(be_plain_font);
//				fOffView->SetHighColor(0, 0, 0);
//				fOffView->DrawString(fAboutText, p);
//
//				if (--fState2 < fPStop)
//				{
					fState1++;
					fState2 = kPEOffsetV;
					fVelocity = 1;
					fPStop = b.left + kPEOffsetH + fPFont.StringWidth("P");
				}
//					fState2 = b.right;
				break;
			}

			case 8:
			{
//				fOffView->FillRect(b, B_SOLID_LOW);

//				fPFont.SetRotation(fVelocity * 10);
//				fEFont.SetRotation(360 - fVelocity * 10);

				BPoint p;
				p.y = kPEOffsetV;
				p.x = b.left + kPEOffsetH;

				fOffView->SetFont(&fPFont);
				fOffView->SetHighColor(0x39, 0x74, 0x79, 255);
				fOffView->DrawString("P", p);

				p.y = fState2;
//				p.x += fPFont->StringWidth("P");
				p.x = fPStop;

				fOffView->SetHighColor(0xa1, 0x64, 0x0e, 255);
				fOffView->DrawString("e", p);

				fState2 += (fVelocity *= 1.05);
				if (fState2 > b.bottom + 30)
				{
					fState1++;
					fState2 = kPEOffsetV;
					fVelocity = 1;
				}
				break;
			}

			case 9:
			{
//				fOffView->FillRect(b, B_SOLID_LOW);

				BPoint p;
				p.y = fState2;
				p.x = b.left + kPEOffsetH;

				fOffView->SetFont(&fPFont);
				fOffView->SetHighColor(0x39, 0x74, 0x79, 255);
				fOffView->DrawString("P", p);

				fState2 += (fVelocity *= 1.05);
				if (fState2 > b.bottom + 30)
				{
					fState1++;
					fState2 = 0;
					fVelocity = 1;

					fPFont.SetSize(32);
					fOffView->SetFont(&fPFont);

					fPStop = b.left + (b.Width()
						- fPFont.StringWidth(kAboutText2)) / 2;
				}
				break;
			}

			case 10:
			{
//				fOffView->FillRect(b, B_SOLID_LOW);

				BPoint p;
				p.y = fState2;
				p.x = fPStop;

#ifdef __HAIKU__
				fOffView->SetHighColor(0x85, 0x19, 0x19, 255);
				fOffView->DrawString(kAboutText2, p);
#else
				fOffView->SetHighColor(0, 0, 80);
				fOffView->DrawString("B", p);
				fOffView->SetHighColor(178, 0, 0);
				fOffView->DrawString("e");
				fOffView->SetHighColor(0, 0, 0);
				fOffView->DrawString("OS ");
				fOffView->SetHighColor(0x85, 0x19, 0x19, 255);
				fOffView->DrawString(kAboutText2 + 5);
#endif

				fState2 += (fVelocity *= 1.04);

				if (fState2 > kPEOffsetV)
				{
					fState1++;
					fState2 = 0;
				}
				break;
			}

			case 11:
			{
				BPoint p;
				p.y = kPEOffsetV;
				p.x = fPStop;

#ifdef __HAIKU__
				fOffView->SetHighColor(0x85, 0x19, 0x19, 255);
				fOffView->DrawString(kAboutText2, p);
#else
				fOffView->SetHighColor(0, 0, 80);
				fOffView->DrawString("B", p);
				fOffView->SetHighColor(178, 0, 0);
				fOffView->DrawString("e");
				fOffView->SetHighColor(0, 0, 0);
				fOffView->DrawString("OS ");
				fOffView->SetHighColor(0x85, 0x19, 0x19, 255);
				fOffView->DrawString(kAboutText2 + 5);
#endif

				if (++fState2 > 100)
				{
					fState1++;
					fState2 = 0;
				}
				break;
			}

			case 12:
			{
				BPoint p;
				p.y = kPEOffsetV;
				p.x = fPStop;

#ifndef __HAIKU__
				unsigned int c = (int)fState2 * 10;
				fOffView->SetHighColor(c, c, (80 * (25 - fState2) + fState2 * 250) / 25);
				fOffView->DrawString("B", p);
				fOffView->SetHighColor((178 * (25 - fState2) + fState2 * 250) / 25 , c, c);
				fOffView->DrawString("e");
				fOffView->SetHighColor(c, c, c);
				fOffView->DrawString("OS ");
#endif

				rgb_color clr = { 0x85, 0x19, 0x19, 255 };
				clr.red = (uint8)((clr.red * (25 - fState2) + fState2 * 250) / 25);
				clr.green = (uint8)((clr.green * (25 - fState2) + fState2 * 250) / 25);
				clr.blue = (uint8)((clr.blue * (25 - fState2) + fState2 * 250) / 25);

				fOffView->SetHighColor(clr);
#ifdef __HAIKU__
				fOffView->DrawString(kAboutText2, p);
#else
				fOffView->DrawString(kAboutText2 + 5);
#endif

				if (++fState2 > 25)
				{
					fState1++;
					fState2 = 0;
				}
				break;
			}

			case 13:
			{
				fOffView->SetDrawingMode(B_OP_COPY);

				if (++fState2 > 10)
				{
					fState1 = 0;
					fState2 = 0;
					fVelocity = 10;
					fPFont.SetShear(45);
					fPFont.SetSize(60);
					fOffView->SetFont(&fPFont);
				}
				break;
			}

			default:
				return;
		}

		fOffView->Sync();
		fOffscreen->Unlock();
	}

	if (Lock())
	{
//b.PrintToStream();
		fView->Draw(b);
		Unlock();
	}

	PostMessage(msg_Animate);
} /* PAboutBox::Animate */

void PAboutBox::ResizeAndCenter(BRect textRect)
{
	BScreen screen(this);
	BRect screenFrame = screen.Frame();
	int32 width = textRect.IntegerWidth() + 20;
	int32 height = textRect.IntegerWidth() * 4 / 5;

	MoveTo((screenFrame.Width() - width) / 2,
		(screenFrame.Height() - height) / 2);
	ResizeTo(width, height);
} /* PAboutBox::ResizeAndCenter */

void PAboutBox::DetermineBounds(BRect &textRect)
{
	font_height fh;
	be_plain_font->GetHeight(&fh);
	float lineHeight = ceilf(fh.ascent + fh.descent + fh.leading);

	const char *text = fAboutText;
	const char *lastLine = text;
	float maxWidth = 0;
	int lineCount = 0;

	do
	{
		if (*text++ == '\n') {
			lineCount++;
			float lineWidth = be_plain_font->StringWidth(lastLine,
				text - 1 - lastLine);
			if (lineWidth > maxWidth)
				maxWidth = lineWidth;

			lastLine = text;
		}
	}
	while (*text);

	textRect.Set(0, 0, maxWidth, lineHeight * lineCount + 2 * kAboutTextHeight);
} /* PAboutBox::DetermineBounds */

void PAboutBox::DrawText(const char *txt, BView *view)
{
	view->FillRect(view->Bounds(), B_SOLID_LOW);
	view->SetFont(be_plain_font);

	font_height fh;
	be_plain_font->GetHeight(&fh);
	float lh = ceil(fh.ascent + fh.descent + fh.leading);

	const char *lp, *ep;
	float l = kAboutTextHeight + fh.ascent + fh.leading, w;

	lp = txt;

	while (*lp)
	{
		ep = strchr(lp, '\n');

		if (ep == NULL)
			break;
		else if (ep == lp)
			; // empty line
		else if (strncmp(lp, "-", ep - lp) == 0)
		{
			view->MovePenTo(0, l - lh / 2);
			view->StrokeLine(BPoint(view->Bounds().Width(), l - lh / 2));
		}
//		else if (strncmp(lp, "--date--", ep - lp) == 0)
//		{
//			w = be_plain_font->StringWidth(__DATE__);
//			view->MovePenTo((kAboutTextWidth - w) / 2, l);
//			view->DrawString(__DATE__);
//		}
		else
		{
			w = be_plain_font->StringWidth(lp, ep - lp);
			view->MovePenTo((view->Bounds().Width() - w) / 2, l);
			view->DrawString(lp, ep - lp);
		}

		l += lh;
		lp = ep + 1;
	}
} /* PAboutBox::DrawText */

// #pragma mark - PAboutView

PAboutView::PAboutView(BRect frame, BBitmap *offscreen, BBitmap *offText)
	: BView(frame, "about view", 0, B_WILL_DRAW)
{
	fOffscreen = offscreen;
	fOffText = offText;
} /* PAboutView::PAboutView */

void PAboutView::Draw(BRect /*update*/)
{
	DrawBitmap(fOffscreen, BPoint(0, 0));
} /* PAboutView::Draw */

void PAboutView::MouseDown(BPoint /*where*/)
{
	Window()->PostMessage(B_QUIT_REQUESTED);
} /* PAboutView::MouseDown */

// #pragma mark - Standalone part only

#if STANDALONE
main()
{
	new BApplication("application/x-vnd.pe-AboutTest");
	new PAboutBox;

	be_app->Run();
	delete be_app;
	return 0;
}
#endif
