# Updating

## Posts

In case of API or theme changes, all of the `index.html` files for every post have to be changed.

To update yours, take the current `page_index.h` and run it through this sed command:

```bash
sed '1,/static char \*PAGE_INDEX =/d; s#\" VERSION \"#'$(
            sed -n 's/^#define VERSION\s*\"\([^\"]*\)\"/\1/p' cgi-bin/include/version.h \
                | sed 's/\/\/.*//' | sed 's/^[ \t]*//;s/[ \t]*$//'
        )'#; s/^[ \t]*\"//g; s/\\n\"$//g; s/\\n\";$//g; s/\\\\/\\/g' \
            cgi-bin/include/page_index.h \
                > /tmp/index.html
```

Then run this command to update all your posts:

```bash
find ~/public/towertalk/posts/ -type f -name index.html -exec cp /tmp/index.html {} \;
```
