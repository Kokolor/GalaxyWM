make
sudo cp shapewm /usr/bin/

cat << EOF > /usr/bin/shapewm-session
#!/bin/sh
xsetroot -solid "#5100ff"
xrdb -load $HOME/.Xdefaults
python3 $HOME/Galaxy/Apps/Terminal/Term.py &
./SetGui.sh &
exec /usr/bin/shapewm
EOF

chmod a+x /usr/bin/shapewm-session

cat << EOF > /usr/share/xsessions/shapewm-session.desktop
[Desktop Entry]
Encoding=UTF-8
Name=ShapeWM
Exec=shapewm-session
Terminal=False
TryExec=shapewm
Type=Application

[Window Manager]
SessionManaged=true
EOF
