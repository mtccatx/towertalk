# Tower Talk

## Description

A small forum-style website for Moontower Computer Club members to post about projects and comment/collaborate.

### How to Use

#### Posting

To post a new project/entry:

- Visit your main towertalk page via `https://moontowercomputer.club/~<your username>/towertalk`
- Click "New Post"
- Enter a title, Markdown body, and your password (See "Installing")
- Click submit

Under the hood, this will call the `towertalk.bin` file in your cgi-bin directory which will:

- Verify your identity by comparing your password with the salt and hash
- Create an Id for the post
- Create a folder `~/public/towertalk/posts/<id>`
- Create a file called `TITLE.txt` in that folder with the title your gave your post
- Create a file called `BODY.md` in that folder with the markdown body you gave your post
- Create a file called `index.html` to render everything

When you then visit `https://moontowercomputer.club/~<your username>/towertalk`, you should see a post there, and if you click it, you'll be taken to `https://moontowercomputer.club/~<your username>/towertalk/posts/<id>` which will let any user view the post.

#### Commenting

Once a post exists and a user can visit it, that user can contribute to the conversation via comments.

To add a comment:

- Visit the post you want to comment on: `https://moontowercomputer.club/~<your username>/towertalk/posts/<id>`
- Click "Add Comment"
- Enter a Markdown body, your username, and your password
- Click submit

Under the hood, this will call the `towertalk.bin` file in the *commenter's* cgi-bin directory which will then:

- Verify their identity
- Create a folder `~/public/towertalk/comments/<id>`
- Create a file called `COMMENT.<number>.md` with the content of the comment in it
   + `<number>` is simply there to give a unique file name in case there are multiple files. It's just a counter

Comments can be viewed under the original post on its page and all the posts can be viewed under your main towertalk page.

### Installing

1. Download latest release. Will have the following inside:
   - Folder with HTML/JS/CSS called `towertalk/`
   - Binary: `towertalk.bin`
2. Install `towertalk.bin` to `~/cgi-bin/`
3. Install the `towertalk/` folder to `~/public`
4. Create a password for using your towertalk
   - Create the folder: `mkdir -p ~/.config/towertalk`
   - Run `openssl passwd -6`
   - This will give you a salt and hashed password in the form: `$6$<salt>$<hash>`
   - Save these under `~/.config/towertalk/password.salt` and `~/.config/towertalk/password.hash`
      * Make sure to add a `\n` at the end!
      * Recommended:
         + `echo "$6$<generated salt>" > ~/.config/towertalk/password.salt`
         + `echo "<generated hash>" > ~/.config/towertalk/password.hash`
