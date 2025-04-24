# Cgi-bin API

## Description

How the underlying cgi-bin HTTP requests work (i.e. the backend).

First of all, every command is a single digit number (for now). Then you follow it with the arguments which are wrapped in curly braces, e.g. `#{arg1}{arg2}`. Always a POST request

## API Commands

The commands are as follows:

- `0` - Make a new Post for this user. Arguments:
   1. Post title
   2. Post body
   3. Poster's user password (`~/.config/towertalk`)
- `1` - Make a comment on a Post
   1. Post Id
   2. Comment body
   3. Commenter's user password (under `~/.config/towertalk`)
- `2` - Return list of Post titles and indices. No args

## Testing

### Curl

To test "properly"/from-a-user-pov, copy your binary to your cgi-bin/ and use curl to send requests:

`curl -X POST -H "Content-Type: text/plain" -d "<command>`

### VS Code Debug

Using the VS Code Debugger, you can change the environment variables and text passed into the program in launch.json to trigger the command.

I.e. change:

```
"name": "CONTENT_LENGTH", "value": "<whatever>"
```

where whatever is the length of the request text you want to test

Then enter whatever your request text is into the terminal.
