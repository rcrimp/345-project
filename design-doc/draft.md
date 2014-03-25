345 Group Assignment Part 1 - 4th April
=======================================

Team
-------------------
- Richie Mckee
- Thomas Hall
- Chris McMillan
- Reuben Crimp
- Shaun Wratten

TODO:
-------------------
- Research what is needed to build this app 
- Design your app, including the scripting language 

Hand in: 
- [ 1 page ] Team background 
- [ 1 page ] Description of the tools/hardware/software needed 
- [ 3 page ] A design of your app (mock-ups, script syntax/semantics)
- [ 1 page ] A schedule 

*Be creative! Think about how primitive a shell is and how you might want to improve on the existing models. Marks will be 
available for creativity as well as satisfying the “letter of the assignment”. The way to think about these creativity marks is 
this: Convince us to switch from the shell we currently use to your shell. 
No one person could do all of this on their own. You must work as a team. Remember, don't be too ambitious”*


Details your team a little background on them. 1 Page
=====================================================
- Reuben Crimp, 19 yrs
Started programming in secondary school, mainly making simple games, and simple websites.
Did well in mathematics, but less so in English. I enrolled at Otago University mid 2012, majoring in CS, with a minor in math.
I have 0% industry experience, so I’m probably as naive as they come. 
I’m the sort of nerd that does math for fun, also vidja games.

- Thomas Hall, 22 yrs.
A recent graduate of a BSc(Physics), I am studying a DipGrad(Computer Science) to be finished early 2015. I have experience programming in C# from messing around with Unity, and Matlab as part of my physics education. I also have limited knowledge of Java and Python from completing the 100 level COMP courses. 

- Chris McMillan, 22 yrs.
Studied at University of Otago since 2010, only became really interested in computers after high school. I completed a chemistry degree and then decided my passion lay in computers and began study towards a computer science major as well. I will finish my BSc double major at the end of 2014. My main strengths are music and audio software. 
Major(s): Chemistry and CS, Minor: Music Technology
I can confidently write java and python programs, but building a shell in C++ will be a challenge that I look forward to and hope to learn a lot from.

- Richie McKee, 26 yrs.
I completed my LLB/BA at the end of 2012 at Otago. I’ve always had a strong desire to learn computer science but only now taken the step towards doing so. I began my dipGrad at the start of 2014 during Summer School and should be finished by the end of 2014. As such my experience with programming.is limited to what I’ve learnt during COMP160 so I’ve never written anything more complex than the java programs from the 160 labs. 

- Shaun, 24 yrs
I recently graduated from UCOL in Palmerston North with a Bachelor’s degree in Information and Communications Technology, in which I learnt a decent portion of my skill set. This included programming in C#, C++, scripting and web design using PHP and Javascript, and working with MS-SQL and MySQL databases. One of the major things I learnt while studying was how to structure a team project, and how to be either a successful project leader or team member.

I have created a website to showcase some of my skills, which can be viewed via http://thefulla.freewebhost.co.nz/ .


A description of the tools/hardware necessary. 1 Page
=====================================================
Version Control:
-------------------
Use Git as distributed revision control, with github for project hosting.

Windows
-------------------
- IDE/compiler: Visual Studio 2013 Professional
- lang: standard c++ libs and standard windows libs included with VS’13.
- GUI: ??

OSX
-------------------
- IDE/compiler: Xcode
- lang: standard C++ libraries
- GUI: Cocoa graphics, in Objective C

Linux
-------------------
- compiler: gcc/g++
- lang: standard C++ libraries
- GUI: X11
- OS versions: 64bit - Fedora, ubuntu, Mint…. 

*The GUI for each platform will be designed separately, to target each platform independently.*

A design of your app 3 Pages
=========================
Mock-ups 
-insert words here-
Features
-------------------
Windows, Linux, OSX platforms
Pipes and redirects | < >
Support proportional spaced fonts for the CLI
Highlight text, cut, copy, paste
Mouse interaction
Syntax highlighting, (gre, grep)

all bash shortcuts
all standard bash commands

support aliases (user defined command shortcuts)
resizable window
Associate *.sh files with our shell

Optional Features
-------------------

A function called “woman”, which is like “man”, but clear and concise (noob friendly)
cd into wd.
i.e.
/home/uname/Documents/coursework/345/project/spec/
cd Documents
/home/uname/Documents/

same line Tab completion i.e like zsh
Utilizes regular expressions
Tab Tab, to list all possible completions

Cross OS compatability
-------------------
Different operating systems by convention define a new line within a file differently
We will allow the user to choose how to handle cross platform files
Fix:        edit as current OS, save as current OS (Forced when parsing a script)
Compatibility:     edit as current OS, save as original.
Do nothing:     edit as original save as original.
Custom:     Fix; but you can specify the replacement terminator {‘\r’, ‘\n’, ‘\r\n’, ‘\n\r’}

Script syntax and semantics definition
like python, but with braces? i.e. fuck the strict use of whitespace.
like Bash, but more strict, like C? i.e. fuck the vague million+ ways to do things

A schedule 1 Page
=================
-insert gantt or pert chart here-

- lots of research
- lots of planning
- 4th April - This document will be done
- more planning
- lots of coding
- 19th May - We will have a working alpha
- coffee
- more coding
- ~28 July - Finished working program
- chill
