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
	
	Created: 02/23/98 20:50:08
*/

#include <vector>

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <Message.h>
#include <Roster.h>
#include <String.h>

const long msg_CommandLineOpen = 'Cmdl';

static std::vector<int> threads;

static BString sTempFilePath;

void DoError(const char *e, ...);
void Usage(bool error);
void OpenInPe(entry_ref& ref, int lineNr);
void OpenInPeDiff(const BString filename1, const BString filename2);

void Usage(bool error)
{
	puts("usage: lpe [--type <fileType>] [file:linenr | +linenr [file] | file] "
		"...");
	puts("If no file has been specified, copy stdin to a temporary file and");
	puts("open that. In that case <fileType> specifies the file extension to");
	puts("be used to help Pe recognize the content type.");
	puts("Alternatively: lpe --diff file1 file2");
	puts("Will open Pe in \"diff mode\" if it is not already running.");
	exit(error ? 1 : 0);
} /* Usage */

void DoError(const char *e, ...)
{
	char msg[1024] = "### lpe Error\n# ";
	
	va_list vl;
	va_start(vl, e);
	vsprintf(msg + strlen(msg), e, vl);
	va_end(vl);
	
	strcat(msg, "\n");

	fputs(msg, stderr);
	exit(1);
} /* error */

void OpenInPe(entry_ref& doc, int lineNr)
{
	BMessage msg(msg_CommandLineOpen), reply;
	msg.AddRef("refs", &doc);
	
	if (lineNr >= 0)
		msg.AddInt32("line", lineNr);

	entry_ref pe;
	if (be_roster->FindApp("application/x-vnd.beunited.pe", &pe))
		DoError("Could not find Pe!");
	
	status_t err;
	if (!be_roster->IsRunning(&pe))
	{
		team_id team;
		err = be_roster->Launch(&pe, &msg, &team);
		if (err) DoError("Error launching Pe (%s)", strerror(err));
		// now wait for the requested edit-window to come up (filter out standard
		// windows):
		bool foundThread = false;
		while (!foundThread)
		{
			int32 cookie = 0;
			thread_info tinfo;
			while ((err = get_next_thread_info(team, &cookie, &tinfo)) == B_OK)
			{
				if (!strncmp("w>", tinfo.name, 2) 
					&& strcmp("w>Pe: Open", tinfo.name)
					&& strcmp("w>Find", tinfo.name)
					&& strcmp("w>/boot/home/config/settings/pe", tinfo.name))
				{
					threads.push_back(tinfo.thread);
					foundThread = true;
					break;
				}
			}
			snooze(100*1000);
		}
	} 
	else 
	{
		BMessenger msr(NULL, be_roster->TeamFor(&pe));
		msr.SendMessage(&msg, &reply);

		if (reply.HasInt32("thread"))
		{
			thread_id tid;
			err = reply.FindInt32("thread", (int32*)&tid);
			if (err) DoError("Error getting thread id (%s)", strerror(err));
		
			threads.push_back(tid);
		}
		else
			DoError("No Thread ID in reply");
	}

} /* OpenInPe */


void OpenInPeDiff(const BString filename1, const BString filename2)
{
	entry_ref pe;
	if (be_roster->FindApp("application/x-vnd.beunited.pe", &pe))
		DoError("Could not find Pe!");

	if (be_roster->IsRunning(&pe))
		DoError("Cannot invoke diff mode when Pe is already running.");

	BMessage msg(B_ARGV_RECEIVED), reply;
	msg.AddInt32("argc", 4);
	msg.AddString("argv", "Pe");
	msg.AddString("argv", "--diff");
	msg.AddString("argv", filename1);
	msg.AddString("argv", filename2);

	team_id team;
	status_t err = be_roster->Launch(&pe, &msg, &team);
	if (err) DoError("Error launching Pe (%s)", strerror(err));
}


static void
remove_temp_file()
{
	if (!sTempFilePath.IsEmpty())
		unlink(sTempFilePath);
}


int main(int argc, char *argv[])
{
	int			i = 0;
	char	 	*p;
	char		*dpPtr;
	int			lineNr = -1;
	status_t	err;
	BEntry		e;
	BString		path;
	BString		path2;
	int			nr;
	bool		pathSeen = false;
	bool		callDiff = false;
	const char*	fileType = NULL;

	while (++i < argc)
	{
		switch (argv[i][0])
		{
			case '+':
				lineNr = strtoul(argv[i] + 1, &p, 10);
				if (!p || p == argv[i] + 1) Usage(true);
				break;

			case '-':
				if (strcmp(argv[i], "-h") == 0
					|| strcmp(argv[i], "--help") == 0) {
					Usage(false);
				}

				if (strcmp(argv[i], "--type") == 0) {
					i++;
					if (i >= argc)
						Usage(true);

					fileType = argv[i];
					break;
				}

				if (strcmp(argv[i], "--diff") == 0) {
					callDiff = true;
					break;
				}

				// fall through

			default:
			{
				pathSeen = true;
				if (!callDiff)
				{
					path = argv[i];
				}
				else
				{
					if (path.IsEmpty())
					{
						path = argv[i];
						break;
					}
					else
					{
						path2 = argv[i];
						break;
					}
				}
				dpPtr = strrchr(argv[i], ':');
				if (dpPtr != NULL)
				{
					nr = strtoul(dpPtr + 1, &p, 10);
					if (strlen(p) == 0)
					{
						path.SetTo(argv[i], dpPtr-argv[i]);
						lineNr = nr;
					}
				}

				err = e.SetTo(path.String(), true);
				if (err) DoError("Error trying to access file %s, (%s)", path.String(), strerror(err));
//				if (! e.Exists()) DoError("File %s does not exist", path.String());
				if (e.Exists() && ! e.IsFile()) DoError("%s is not a regular file", path.String());
				
				entry_ref ref;
				err = e.GetRef(&ref);
				if (err) DoError("Error trying to access file %s, (%s)", path.String(), strerror(err));
				OpenInPe(ref, lineNr);
				lineNr = -1;
			}
		}
	}

	if (callDiff)
	{
		if (path.IsEmpty() || path2.IsEmpty())
			Usage(true);
		OpenInPeDiff(path, path2);
		return 0;
	}

	if (!pathSeen) {
		// No files specified. Write the stdin to a file and open that instead.

		sTempFilePath.SetToFormat("/tmp/lpe-%ld", (long)find_thread(NULL));
		if (fileType != NULL)
			sTempFilePath << '.' << fileType;

		entry_ref tempFileRef;
		status_t error = get_ref_for_path(sTempFilePath, &tempFileRef);
		if (error != B_OK) {
			DoError("Failed to get temporary file entry ref: %s",
				strerror(error));
		}

		// remove and re-create the file
		unlink(sTempFilePath);

		int fd = creat(sTempFilePath, S_IRUSR | S_IWUSR);
		if (fd < 0)
			DoError("Failed to create temporary file: %s", strerror(errno));

		atexit(&remove_temp_file);

		// copy stdin to file
		char buffer[64 * 1024];
		for (;;) {
			size_t bytesRead = fread(buffer, 1, sizeof(buffer), stdin);
			if (bytesRead == 0) {
				if (ferror(stdin) != 0) {
					error = errno;
					DoError("Failed to read from stdin: %s", strerror(error));
				}
				break;
			}

			ssize_t bytesWritten = write(fd, buffer, bytesRead);
			if (bytesWritten < 0) {
				error = errno;
				DoError("Failed to write to temporary file: %s",
					strerror(error));
			}

			if ((size_t)bytesWritten != bytesRead) {
				DoError("Failed to write to temporary file: unknown error",
					strerror(error));
			}
		}

		close(fd);

		OpenInPe(tempFileRef, lineNr);
	}

	std::vector<int>::iterator ti;
	
	for (ti = threads.begin(); ti != threads.end(); ti++)
	{
		status_t l;
		wait_for_thread((*ti), &l);
//		status_t err = wait_for_thread((*ti), &l);
//		if (err) DoError("Error waiting for thread: %s", strerror(err));
	}
	
	return 0;
} /* main */
