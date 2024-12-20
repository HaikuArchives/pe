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
*/

#include "Types.r"
#include "KeyBindings.h"
#include "PMessages.h"
#include "ResourcesBindings.h"


//		kmsg_OpenHeaderSource, { Shift, '\t' }, "Open Header/Source File",
//		"Abort Shell Command",

resource rtyp_Cmnd (rid_Cmnd_Editing, "Editing commands") {
	{
		kmsg_NrArgument,							"Numerical argument prefix",
		kmsg_Recenter,								"Recenter",
		kmsg_OpenLine,								"Open line",
		kmsg_AppendNextCut,							"Append next cut",
		kmsg_ClearRegion,							"Delete region between caret and mark",
		kmsg_CopyRegion,							"Copy region between caret and mark",
		kmsg_CutRegion,								"Cut region between caret and mark",
		kmsg_ExchangeMarkAndPoint,					"Exchange caret and mark",
		kmsg_MarkWord,								"Mark word",
		kmsg_MarkAll,								"Mark all",
		kmsg_Mark,									"Mark",
		kmsg_CutWordBackward,						"Cut previous word",
		kmsg_CutWord,								"Cut next word",
		kmsg_CutToEndOfLine,						"Cut to end of line",
		kmsg_DeleteToEndOfFile,						"Delete to end of file",
		kmsg_DeleteToEndOfLine,						"Delete to end of line",
		kmsg_DeleteToBeginningOfLine,				"Delete to beginning of line",
		kmsg_DeleteCharacterRight,					"Delete character right",
		kmsg_DeleteCharacterLeft,					"Delete character left",
		kmsg_ScrollToEndOfFile,						"Scroll to end of file",
		kmsg_ScrollToStartOfFile,					"Scroll to start of file",
		kmsg_ScrollPageDown,						"Scroll page down",
		kmsg_ScrollPageUp,							"Scroll page up",
		kmsg_ScrollOneLineDown,						"Scroll one line down",
		kmsg_ScrollOneLineUp,						"Scroll one line up",
		kmsg_ExtendSelectionToEndOfFile,			"Extend selection to end of file",
		kmsg_ExtendSelectionToBeginningOfFile,		"Extend selection to beginning of file",
		kmsg_ExtendSelectionToEndOfPage,			"Extend selection to end of page",
		kmsg_ExtendSelectionToBeginningOfPage,		"Extend selection to beginning of page",
		kmsg_ExtendSelectionToNextPage,				"Extend selection to next page",
		kmsg_ExtendSelectionToPreviousPage,			"Extend selection to previous page",
//		kmsg_ExtendSelectionToCurrentLine,			"Extend selection to current line",
		kmsg_ExtendSelectionToNextLine,				"Extend selection to next line",
		kmsg_ExtendSelectionToPreviousLine,			"Extend selection to previous line",
		kmsg_ExtendSelectionToEndOfLine,			"Extend selection to end of line",
		kmsg_ExtendSelectionToBeginningOfLine,		"Extend selection to beginning of line",
		kmsg_ExtendSelectionWithNextSubword,		"Extend selection with next sub-word",
		kmsg_ExtendSelectionWithPreviousSubword,	"Extend selection with previous sub-word",
		kmsg_ExtendSelectionWithNextWord,			"Extend selection with next word",
		kmsg_ExtendSelectionWithPreviousWord,		"Extend selection with previous word",
		kmsg_ExtendSelectionWithCharacterRight,		"Extend selection with character right",
		kmsg_ExtendSelectionWithCharacterLeft,		"Extend selection with character left",
		kmsg_MoveToEndOfFile,						"Move to end of file",
		kmsg_MoveToBeginningOfFile,					"Move to beginning of file",
		kmsg_MoveToBottomOfPage,					"Move to bottom of page",
		kmsg_MoveToTopOfPage,						"Move to top of page",
		kmsg_MoveToNextPage,						"Move to next page",
		kmsg_MoveToPreviousPage,					"Move to previous page",
		kmsg_MoveToNextLine,						"Move to next line",
		kmsg_MoveToPreviousLine,					"Move to previous line",
		kmsg_MoveToEndOfLine,						"Move to end of line",
		kmsg_MoveToBeginningOfLine,					"Move to beginning of line",
		kmsg_MoveWordRight,							"Move word right",
		kmsg_MoveWordLeft,							"Move word left",
		kmsg_MoveSubwordRight,						"Move sub-word right",
		kmsg_MoveSubwordLeft,						"Move sub-word left",
		kmsg_MoveCharacterRight,					"Move character right",
		kmsg_MoveCharacterLeft,						"Move character left"
	}
};

resource rtyp_Cmnd (rid_Cmnd_FileMenu, "File menu") {
	{
		msg_Quit,									"Quit",
		msg_Revert,									"Revert",
		msg_SaveCopy,								"Save a copy as…",
		msg_FtpSave,								"Save on server…",
		msg_SaveAs,									"Save as…",
		msg_SaveAll,								"Save all",
		msg_Save,									"Save",
		msg_CloseAll,								"Close all",
		msg_Close,									"Close",
		msg_OpenSelected,							"Open selected file",
		msg_FtpOpen,								"Open from server…",
		msg_Open,									"Open…",
		msg_NewGroup,								"New group",
		msg_New,									"New"
	}
};

resource rtyp_Cmnd (rid_Cmnd_EditMenu, "Edit menu") {
	{
		msg_SelectParagraph,						"Select paragraph",
		msg_SelectLine,								"Select line",
		'SALL',										"Select all",
		msg_Clear,									"Clear",
		'PSTE',										"Paste",
		msg_CopyAppend,								"Copy and append",
		'COPY',										"Copy",
		msg_CutAppend,								"Cut and append",
		'CCUT',										"Cut",
		msg_Redo,									"Redo",
		msg_Undo,									"Undo"
	}
};

resource rtyp_Cmnd (rid_Cmnd_TextMenu, "Text menu") {
	{
		msg_ShowInBrowser,							"Show in browser",
		msg_Unwrap,									"Remove line breaks",
		msg_Wrap,									"Insert line breaks",
		msg_Justify,								"Justify",
		msg_Uncomment,								"Uncomment",
		msg_Comment,								"Comment",
		msg_ShiftRight,								"Shift right",
		msg_ShiftLeft,								"Shift left",
		msg_ConvertEncoding,						"Change encoding…",
		msg_ChangeCaseSent,							"Capitalize sentences",
		msg_ChangeCaseLine,							"Capitalize lines",
		msg_ChangeCaseCap,							"Capitalize words",
		msg_ChangeCaseLower,						"Change to lowercase",
		msg_ChangeCaseUpper,						"Change to uppercase",
		msg_Twiddle,								"Twiddle",
		msg_Balance,								"Balance"
	}
};

resource rtyp_Cmnd (rid_Cmnd_SearchMenu, "Search menu") {
	{
		msg_GoToLine,								"Go to line…",
		msg_FindNextError,							"Jump to next error",
		msg_IncSearchBackward,						"Incremental search backwards",
		msg_IncSearch,								"Incremental search",
		msg_FindInNextFile,							"Find in next file",
		msg_ReplaceAll,								"Replace all",
		msg_ReplaceAndFindBackward,					"Replace and find backwards",
		msg_ReplaceAndFind,							"Replace and find",
		msg_Replace,								"Replace…",
		msg_EnterReplaceString,						"Enter replace string",
		msg_EnterSearchString,						"Enter search string",
		msg_FindSelectionBackward,					"Find selection backwards",
		msg_FindSelection,							"Find selection",
		msg_FindAgainBackward,						"Find again backwards",
		msg_FindAgain,								"Find again",
		msg_FindCmd,								"Find…",
		msg_FindFunction,							"Find function…",
		msg_PreviousFunction,						"Previous function",
		msg_NextFunction,							"Next function"
	}
};

resource rtyp_Cmnd (rid_Cmnd_WindowMenu, "Window menu") {
	{
		msg_Worksheet,								"Open worksheet",
		msg_Zoom,									"Zoom",
		msg_Tile,									"Tile",
		msg_Stack,									"Stack",
		msg_ShowHtmlPalette,						"HTML palette",
		msg_ShowGlossary,							"Glossary",
		msg_ChangeWorkingDir,						"Working directory…",
		msg_Preferences,							"Preferences…",
		msg_Info,									"File options…",
	}
};

resource rtyp_Cmnd (rid_Cmnd_HelpMenu, "Help menu") {
	{
		msg_About,									"About…",
		msg_Help,									"Help…"
	}
};

resource rtyp_Cmnd (rid_Cmnd_BeideMenu, "BeIDE menu") {
	{
		msg_IdeMake,								"Make",
		msg_IdeProjectToGroup,						"Project -> Group",
		msg_IdeRemove,								"Remove file",
		msg_IdeAdd,									"Add file",
		msg_IdeBringToFront,						"Bring to front"
	}
};

resource rtyp_Cmnd (rid_Cmnd_Miscellaneous, "Miscellaneous") {
	{
		kmsg_UnsplitWindow,							"Unsplit window",
		kmsg_SwitchActivePart,						"Switch to other part of window",
		kmsg_SplitWindow,							"Split window",
		msg_CancelCommand,							"Cancel executing command",
		msg_Execute,								"Execute command",
		msg_ExecuteToWorksheet,						"Execute command (w/ output to Worksheet)",
		msg_SwitchHeaderSource,						"Open header/source",
	}
};

resource rtyp_Cmnd (rid_Cmnd_Extensions, "Extensions") {
	{
	}
};

// Ctrl = left Ctrl key on intel keyboard
// Opt = right Ctrl key on intel keyboard
// Cmd = either Alt key on intel keyboard

resource rtyp_Bind (rid_Bind_Editing, "Bindings for editing") {
	{
		0,				0x1c,	0,		0,			kmsg_MoveCharacterLeft,
		0,				0x1d,	0,		0,			kmsg_MoveCharacterRight,
		Cmd,			0x1c,	0,		0,			kmsg_MoveWordLeft,
		Cmd,			0x1d,	0,		0,			kmsg_MoveWordRight,
		Opt,			0x1c,	0,		0,			kmsg_MoveSubwordLeft,
		Opt,			0x1d,	0,		0,			kmsg_MoveSubwordRight,
		0,				0x01,	0,		0,			kmsg_MoveToBeginningOfLine,
		0,				0x04,	0,		0,			kmsg_MoveToEndOfLine,
		0,				0x1e,	0,		0,			kmsg_MoveToPreviousLine,
		0,				0x1f,	0,		0,			kmsg_MoveToNextLine,
		0,				0x0b,	0,		0,			kmsg_MoveToTopOfPage,
		0,				0x0c,	0,		0,			kmsg_MoveToBottomOfPage,
		Cmd,			0x01,	0,		0,			kmsg_MoveToBeginningOfFile,
		Cmd,			0x04,	0,		0,			kmsg_MoveToEndOfFile,
		Opt,			0x01,	0,		0,			kmsg_MoveToBeginningOfFile,
		Opt,			0x04,	0,		0,			kmsg_MoveToEndOfFile,
		0,				0x08,	0,		0,			kmsg_DeleteCharacterLeft,
		Ctrl,			0x08,	0,		0,			kmsg_DeleteToBeginningOfLine,
		0,				0x7f,	0,		0,			kmsg_DeleteCharacterRight,
		Ctrl,			0x7f,	0,		0,			kmsg_DeleteToEndOfLine,
		Shift,			0x1c,	0,		0,			kmsg_ExtendSelectionWithCharacterLeft,
		Shift,			0x1d,	0,		0,			kmsg_ExtendSelectionWithCharacterRight,
		Shift|Cmd,		0x1c,	0,		0,			kmsg_ExtendSelectionWithPreviousWord,
		Shift|Cmd,		0x1d,	0,		0,			kmsg_ExtendSelectionWithNextWord,
		Shift|Opt,		0x1c,	0,		0,			kmsg_ExtendSelectionWithPreviousSubword,
		Shift|Opt,		0x1d,	0,		0,			kmsg_ExtendSelectionWithNextSubword,
		Shift,			0x1e,	0,		0,			kmsg_ExtendSelectionToPreviousLine,
		Shift,			0x1f,	0,		0,			kmsg_ExtendSelectionToNextLine,
		Shift,			0x01,	0,		0,			kmsg_ExtendSelectionToBeginningOfLine,
		Shift,			0x04,	0,		0,			kmsg_ExtendSelectionToEndOfLine,
		Shift,			0x0b,	0,		0,			kmsg_ExtendSelectionToBeginningOfPage,
		Shift,			0x0c,	0,		0,			kmsg_ExtendSelectionToEndOfPage,
		Shift|Cmd,		0x01,	0,		0,			kmsg_ExtendSelectionToBeginningOfFile,
		Shift|Cmd,		0x04,	0,		0,			kmsg_ExtendSelectionToEndOfFile,
		Shift|Opt,		0x01,	0,		0,			kmsg_ExtendSelectionToBeginningOfFile,
		Shift|Opt,		0x04,	0,		0,			kmsg_ExtendSelectionToEndOfFile,
		Ctrl,			0x1e,	0,		0,			kmsg_ScrollOneLineUp,
		Ctrl,			0x1f,	0,		0,			kmsg_ScrollOneLineDown,
		Opt,			0x1e,	0,		0,			kmsg_ScrollOneLineUp,
		Opt,			0x1f,	0,		0,			kmsg_ScrollOneLineDown,
		// keypad-keys:
		0,				0x1c,	0,		0,			kmsg_MoveCharacterLeft,
		0,				0x1d,	0,		0,			kmsg_MoveCharacterRight,
		Cmd,			0x1c,	0,		0,			kmsg_MoveWordLeft,
		Cmd,			0x1d,	0,		0,			kmsg_MoveWordRight,
		Opt,			0x1c,	0,		0,			kmsg_MoveSubwordLeft,
		Opt,			0x1d,	0,		0,			kmsg_MoveSubwordRight,
		0,				0x01,	0,		0,			kmsg_MoveToBeginningOfLine,
		0,				0x04,	0,		0,			kmsg_MoveToEndOfLine,
		0,				0x1e,	0,		0,			kmsg_MoveToPreviousLine,
		0,				0x1f,	0,		0,			kmsg_MoveToNextLine,
		0,				0x0b,	0,		0,			kmsg_MoveToTopOfPage,
		0,				0x0c,	0,		0,			kmsg_MoveToBottomOfPage,
		Cmd,			0x01,	0,		0,			kmsg_MoveToBeginningOfFile,
		Cmd,			0x04,	0,		0,			kmsg_MoveToEndOfFile,
		Opt,			0x01,	0,		0,			kmsg_MoveToBeginningOfFile,
		Opt,			0x04,	0,		0,			kmsg_MoveToEndOfFile,
		0,				0x7f,	0,		0,			kmsg_DeleteCharacterRight,
		Ctrl,			0x7f,	0,		0,			kmsg_DeleteToEndOfLine,
		Shift,			0x1c,	0,		0,			kmsg_ExtendSelectionWithCharacterLeft,
		Shift,			0x1d,	0,		0,			kmsg_ExtendSelectionWithCharacterRight,
		Shift|Cmd,		0x1c,	0,		0,			kmsg_ExtendSelectionWithPreviousWord,
		Shift|Cmd,		0x1d,	0,		0,			kmsg_ExtendSelectionWithNextWord,
		Shift|Opt,		0x1c,	0,		0,			kmsg_ExtendSelectionWithPreviousSubword,
		Shift|Opt,		0x1d,	0,		0,			kmsg_ExtendSelectionWithNextSubword,
		Shift,			0x1e,	0,		0,			kmsg_ExtendSelectionToPreviousLine,
		Shift,			0x1f,	0,		0,			kmsg_ExtendSelectionToNextLine,
		Shift,			0x01,	0,		0,			kmsg_ExtendSelectionToBeginningOfLine,
		Shift,			0x04,	0,		0,			kmsg_ExtendSelectionToEndOfLine,
		Shift,			0x0b,	0,		0,			kmsg_ExtendSelectionToBeginningOfPage,
		Shift,			0x0c,	0,		0,			kmsg_ExtendSelectionToEndOfPage,
		Shift|Cmd,		0x01,	0,		0,			kmsg_ExtendSelectionToBeginningOfFile,
		Shift|Cmd,		0x04,	0,		0,			kmsg_ExtendSelectionToEndOfFile,
		Shift|Opt,		0x01,	0,		0,			kmsg_ExtendSelectionToBeginningOfFile,
		Shift|Opt,		0x04,	0,		0,			kmsg_ExtendSelectionToEndOfFile,
	}
};

resource rtyp_Bind (rid_Bind_FileMenu, "Bindings for file menu") {
	{
		Cmd,			0x71,	0,		0,			msg_Quit,
//		0,				0,		0, 		0,			msg_Revert,
//		0,				0,		0, 		0,			msg_SaveCopy,
//		0,				0,		0, 		0,			msg_SaveAs,
		Cmd|Shift,		0x73,	0,		0,			msg_SaveAll,
		Cmd,			0x73,	0,		0,			msg_Save,
		Cmd|Shift,		0x77,	0,		0,			msg_CloseAll,
		Cmd,			0x77,	0,		0,			msg_Close,
		Cmd,			0x64,	0,		0,			msg_OpenSelected,
		Cmd|Shift,		0x6f,	0,		0,			msg_FtpOpen,
		Cmd,			0x6f,	0,		0,			msg_Open,
//		0,				0,		0, 		0,			msg_NewGroup,
		Cmd,			0x6e,	0,		0,			msg_New
	}
};

resource rtyp_Bind (rid_Bind_EditMenu, "Bindings for edit menu") {
	{
		Cmd,			0x6c,	0,		0,			msg_SelectLine,
		Cmd,			0x61,	0,		0,			'SALL',
		Cmd,			0x76,	0,		0,			'PSTE',
		Cmd,			0x63,	0,		0,			'COPY',
		Cmd|Shift,		0x63,	0,		0,			msg_CopyAppend,
		Cmd,			0x78,	0,		0,			'CCUT',
		Cmd|Shift,		0x78,	0,		0,			msg_CutAppend,
		Cmd|Shift,		0x79,	0,		0,			msg_Redo,
		Cmd,			0x79,	0,		0,			msg_Undo,
		Cmd|Shift,		0x7a,	0,		0,			msg_Redo,
		Cmd,			0x7a,	0,		0,			msg_Undo
	}
};

resource rtyp_Bind (rid_Bind_TextMenu, "Bindings for text menu") {
	{
		Cmd,			0x62,	0,		0,			msg_Balance,
		Cmd,			0x60,	0,		0,			msg_Twiddle,
//		0,				0,		0, 		0,			msg_ChangeCaseUpper,
//		0,				0,		0, 		0,			msg_ChangeCaseLower,
//		0,				0,		0, 		0,			msg_ChangeCaseCap,	
//		0,				0,		0, 		0,			msg_ConvertEncoding,
		Cmd,			0x5b,	0,		0,			msg_ShiftLeft,
		Cmd,			0x5d,	0,		0,			msg_ShiftRight,
//		0,				0,		0, 		0,			msg_Comment,
//		0,				0,		0, 		0,			msg_Uncomment,
//		Cmd,			0x5c,	0,		0,			msg_Wrap,
//		0,				0,		0, 		0,			msg_Unwrap,
		Cmd|Shift,		0x62,	0,		0,			msg_ShowInBrowser
	}
};

resource rtyp_Bind (rid_Bind_SearchMenu, "Bindings for search menu") {
	{
		Cmd,			0x2c,	0,		0,			msg_GoToLine,	
//		Cmd,			0x69,	0,		0,			msg_FindNextError,
		Cmd,			0x6a,	0,		0,			msg_FindInNextFile,
//		0,				0,		0, 		0,			msg_ReplaceAll,
		Cmd|Shift,		0x74,	0,		0,			msg_ReplaceAndFindBackward,
		Cmd,			0x74,	0,		0,			msg_ReplaceAndFind,
		Cmd,			0x3d,	0,		0,			msg_Replace,
		Cmd|Shift,		0x65,	0,		0,			msg_EnterReplaceString,
		Cmd,			0x65,	0,		0,			msg_EnterSearchString,
		Cmd|Shift,		0x68,	0,		0,			msg_FindSelectionBackward,
		Cmd,			0x68,	0,		0,			msg_FindSelection,
		Cmd|Shift,		0x67,	0,		0,			msg_FindAgainBackward,
		Cmd,			0x67,	0,		0,			msg_FindAgain,
		Cmd,			0x66,	0,		0,			msg_FindCmd,
		Cmd,			0x69,	0,	 	0,		 	msg_IncSearch,
		Cmd|Shift,		0x69,	0,		0,			msg_IncSearchBackward,
		Cmd|Shift,		0x66,	0,		0,			msg_FindFunction
	}
};

resource rtyp_Bind (rid_Bind_WindowMenu, "Bindings for window menu") {
	{
//		0,				0,		0, 		0,			msg_FontTabs,
//		0,				0,		0, 		0,			msg_Info,
//		0,				0,		0, 		0,			msg_Preferences,
//		0,				0,		0, 		0,			msg_ChangeWorkingDir,
//		0,				0,		0, 		0,			msg_ShowGlossary,
//		0,				0,		0, 		0,			msg_Stack,
//		0,				0,		0, 		0,			msg_Tile,
		Cmd,			0x2f,	0,		0,			msg_Zoom,
		Cmd,			0x30,	0,		0,			msg_Worksheet
	}
};

resource rtyp_Bind (rid_Bind_HelpMenu, "Bindings for help menu") {
	{
//		0,				0,		0, 		0,			msg_About,
//		0,				B_FUNCTION_KEY,	0,	0,		msg_Help
	}
};

resource rtyp_Bind (rid_Bind_BeideMenu, "Bindings for BeIDE menu") {
	{
		Cmd,			0x6d,	0,		0,			msg_IdeMake,
//		0,				0,		0, 		0,			msg_IdeProjectToGroup,
//		0,				0,		0, 		0,			msg_IdeRemove,
//		0,				0,		0, 		0,			msg_IdeAdd,
//		0,				0,		0, 		0,			msg_IdeBringToFront
	}
};

resource rtyp_Bind (rid_Bind_Miscellaneous, "Miscellaneous bindings") {
	{
		Ctrl,			0x63,	0,		0,			msg_CancelCommand,
		Cmd,			0x0a,	0,		0,			msg_Execute,
		Cmd|Shift,		0x0a,	0,		0,			msg_ExecuteToWorksheet,
//		0,				0x0a,	0,		0,			msg_Execute,
		Cmd,			0x09,	0,		0,			msg_SwitchHeaderSource
	}
};

resource rtyp_Bind (rid_Bind_Emacs, "Emacs bindings") {
	{
		Ctrl,			0x75,	0,		0,			kmsg_NrArgument,
		Ctrl,			0x6c,	0,		0,			kmsg_Recenter,
		Ctrl,			0x6f,	0,		0,			kmsg_OpenLine,
		Ctrl,			0x77,	0,		B_ESCAPE,	kmsg_AppendNextCut,
//		Cmd,			0x09,	0,		0,			kmsg_ClearRegion,	
		0,				0x77,	0,		B_ESCAPE,	kmsg_CopyRegion,
		Ctrl,			0x77,	0,		0,			kmsg_CutRegion,
		Ctrl,			0x78,	Ctrl,	0x78,		kmsg_ExchangeMarkAndPoint,
		Shift,			0x32,	0,		B_ESCAPE,	kmsg_MarkWord,
		0,				0x68,	Ctrl,	0x78,		kmsg_MarkAll,
		Ctrl,			0x20,	0,		0,			kmsg_Mark,
//		Ctrl,			0x7f,	0,		0,			kmsg_CutWordBackward,
		0,				0x64,	0,		B_ESCAPE,	kmsg_CutWord,
		Ctrl,			0x6b,	0,		0,			kmsg_CutToEndOfLine,
		Ctrl,			0x7a,	0,		0, 			'PSTE',
		0,				0x32,	Ctrl,	0x78,		kmsg_SplitWindow,
		0,				0x31,	Ctrl,	0x78,		kmsg_UnsplitWindow,
		0,				0x6f,	Ctrl,	0x78,		kmsg_SwitchActivePart
	}
};
