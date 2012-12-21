This project tests the case when dll is loaded from parent's dir instead of from dir of child exe.

On windows ce 6.0 this vulnerability confirmed.

How to use:
create dll with string resource and make desktop.ini file with the following content:
`[.ShellClassInfo]
LocalizedResourceName=@\Temp\dummy_dll.dll,-102`

where `dummy_dll.dll` - is your dll with string resource with ID 102