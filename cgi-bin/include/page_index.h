// Generated index.html for displaying page content and "Add Comment" functionality

#pragma once

static char *PAGE_INDEX =
    "<!-- Post page for towertalk -->\n"
    "<!DOCTYPE html>\n"
    "<html>\n"
    "    <head>\n"
    "        <script>\n"
    "            const path = window.location.pathname;\n"
    "            const match = path.match(/^\\/~([^/]+)\\//);\n"
    "            const envVar = match ? match[1] : '<user>';\n"
    "            document.title = `Tower Talk - ${envVar}`;\n"
    "        </script>\n"
    "    </head>\n"
    "</html>\n";
