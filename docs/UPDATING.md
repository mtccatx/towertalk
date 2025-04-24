# Updating

## Posts

In case of API or theme changes, all of the `index.html` files for every post have to be changed.

To update yours, take the current `page_index.h` and run it through this sed command:

```bash
sed -n '/^static char \*PAGE_INDEX =/,/^";/p' cgi-bin/include/page_index.h \
    | sed '1d;$d;s/^ *"//;s/\\n"$//;s/"$//;s/\\"/"/g;s/\\\\/\\/g' \
    > /tmp/index.html
```

Then run this command to update all your posts:

```bash
find ~/public/towertalk/posts/ -type f -name index.html -exec cp /tmp/index.html {} \;
```
