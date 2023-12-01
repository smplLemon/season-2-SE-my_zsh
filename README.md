# season-2-SE-my_zsh
<div class="card-block">
<div class="row">
<div class="col tab-content">
<div class="tab-pane active show" id="subject" role="tabpanel">
<div class="row">
<div class="col-md-12 col-xl-12">
<div class="markdown-body">
<p class="text-muted m-b-15">
</p><h1>My Zsh</h1>
<p>Remember to git add &amp;&amp; git commit &amp;&amp; git push each exercise!</p>
<p>We will execute your function with our test(s), please DO NOT PROVIDE ANY TEST(S) in your file</p>
<p>For each exercise, you will have to create a folder and in this folder, you will have additional files that contain your work. Folder names are provided at the beginning of each exercise under <code>submit directory</code> and specific file names for each exercise are also provided at the beginning of each exercise under <code>submit file(s)</code>.</p>
<hr>
<table>
<thead>
<tr>
<th>My Zsh</th>
<th></th>
</tr>
</thead>
<tbody>
<tr>
<td>Submit directory</td>
<td>.</td>
</tr>
<tr>
<td>Submit files</td>
<td>Makefile - *.c - *.h</td>
</tr>
</tbody>
</table>
<h3>Description</h3>
<h2>Introduction</h2>
<p>There are programs out there that everyone uses, and it’s easy to put their developers on a pedestal. Although developing large software projects isn’t easy, many times the basic idea of that software is quite simple. Implementing it yourself is a fun way to show that you have what it takes to be a real programmer.</p>
<p>Write your own Shell, is it possible?
Let's do it! :-)</p>
<img src="https://storage.googleapis.com/qwasar-public/s02_SE/shell_logo_no_bg.png" width="200">
<br>
<br>
<p>Zsh - tcsh - bash - sh are command interpreters. A command interpreter is run inside a <code>terminal</code>.</p>
<p>What is a UNIX command interpreter?
If we have to make it simple: It's a <code>loop</code>.</p>
<p>What is it made of ?</p>
<pre class=" language-plain"><code class=" language-plain">-&gt;
|  Read: Read the command from standard input.
|  Parse: Separate the command string into a program and arguments.
|  Execute: Run the parsed command.
|-- (repeat)
</code></pre>
<p>Each one of those 3 parts leave us with more questions:
I# How to read?
II# How to parse?
III# How to <code>run</code>?</p>
<h2>I Read line</h2>
<p>You should be able to do it ;-)</p>
<h2>II Parse</h2>
<p>You should also be able to google / do it :-)
(A little voice in my head is whispering Lexer - AST?)</p>
<h2>III Execution</h2>
<h3>3.0 Binary vs shell builtins</h3>
<p>In the shell, there is a difference between
ls - cat - tail - head - ...
and pwd - env - setenv - cd - ...</p>
<pre class=" language-plain"><code class=" language-plain">$&gt;which ls
/bin/ls
$&gt;
</code></pre>
<pre class=" language-plain"><code class=" language-plain">$&gt;which pwd
pwd: shell built-in command
$&gt;
</code></pre>
<p>This difference creates two types of execution:</p>
<ul>
<li>builtins are function inside your Shell</li>
<li>binary have to be <code>exec</code> in your shell</li>
</ul>
<h3>3.1 To execute a binary, the <code>exec*()</code> functions.</h3>
<p>This is the way to start a binary (example: ls)
Exec*() have different interface you can use. For this project you will have to use: <code>execve()</code>.</p>
<p>But all <code>exec</code> functions have a similar behaviour, they kill their own process after their execution.
It means you can run one <code>ls</code> and then your shell will die.</p>
<p>How to solve this problem?
You will have to create another process, using a function called: <code>fork()</code>:</p>
<h3>3.2 How to fork()?</h3>
<p>Here is an example of code:</p>
<pre class=" language-plain"><code class=" language-plain">int executor(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  // after this function, execution has now two processes: a parent and a child.
  // how to determine which one is which?
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) { // Executing the command.
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &amp;status, WUNTRACED);
    } while (!WIFEXITED(status) &amp;&amp; !WIFSIGNALED(status));
    // Wait for process child to be done.
  }

  return 1;
}
</code></pre>
<p>Scheme of Fork() - execve() - wait()</p>
<img src="https://storage.googleapis.com/qwasar-public/s02_SE/Wait_system_call_in_c.jpg" width="800">
<h3>3.3 Where to find the different binary?</h3>
<p>This is a great question! We are using the variable PATH from the environment.</p>
<pre class=" language-plain"><code class=" language-plain">$&gt;echo $PATH
...
$&gt;
</code></pre>
<h3>3.4 How to access the environment?</h3>
<p>You might learn something new here:
main() function in C actually receives 3 arguments!</p>
<pre class=" language-plain"><code class=" language-plain">int main(int ac, char **av, char** env) // &lt;- hehe
</code></pre>
<h2>Technical specifications</h2>
<ul>
<li>Your interpreter will display a prompt respecting this format: [what_you_want]&gt;
example with pwd as what_you_want:</li>
</ul>
<pre class=" language-plain"><code class=" language-plain">[/home/gaetan/]&gt;ls
...
[/home/gaetan/]&gt;
</code></pre>
<ul>
<li>
<p>You will have to wait for a command line to be written (ending by a newline character (enter))</p>
</li>
<li>
<p>You will have to parse the command line and format it in order to execute it.</p>
</li>
<li>
<p>The prompt must be displayed again only after the command execution.</p>
</li>
<li>
<p>For the execution you must use <code>execve</code>.</p>
</li>
</ul>
<p>Only basic command lines are expected to be executed; no pipes, redirections or any other advanced features. The executables should be those found in the path, as indicated in the PATH variable.</p>
<p>If the executable cannot be found, you must display an error message and display the prompt again. Errors must be dealt with and must display the appropriate message on the error output. You must correctly handle the PATH and the environment (by copying and restoring the initial env).</p>
<p>You must implement the following builtins: echo, cd, setenv, unsetenv, env, exit, pwd and which.</p>
<h2>Example</h2>
<pre class=" language-console"><code class=" language-console">$&gt;./my_zsh
GTN - my_zsh $&gt;echo "hello"
hello
GTN - my_zsh $&gt;ls -l
Applications		Downloads		Library			Past			Public			Desktop			Dropbox
Movies			Pictures		Sites      Documents
Music			Project			bin			go
GTN - my_zsh $&gt;pwd
/Users/gtn/
GTN - my_zsh $&gt;cd /tmp
GTN - my_zsh $&gt;pwd
/tmp/
GTN - my_zsh $&gt;./my_segfault_program
Segfault
GTN - my_zsh $&gt;cd -
/Users/gtn/
GTN - my_zsh $&gt;exit
$&gt;
</code></pre>
<h2>Authorized functions</h2>
<ul>
<li>malloc, free, exit, opendir, readdir, closedir, getcwd, chdir</li>
<li>fork, stat, lstat, fstat, open, close, getline</li>
<li>read, write, execve, isatty, wait, waitpid</li>
<li>wait3, wait4, signal, kill, getpid, strerror, perror.</li>
</ul>
<h2>Requirements</h2>
<ul>
<li>Your code must be compiled with the flags <strong>-Wall -Wextra -Werror</strong>.</li>
<li>No memory leaks are allowed.</li>
<li>Parser can be a split on spaces.</li>
<li>Multiline macros are forbidden</li>
<li>Include another .c is forbidden</li>
<li>Macros with logic (while/if/variables/...) are forbidden</li>
</ul>

<p></p>
</div>

</div>
</div>
</div>
<div class="tab-pane" id="resources" role="tabpanel">
</div>
</div>
</div>
</div>
