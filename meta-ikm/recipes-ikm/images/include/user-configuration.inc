inherit extrausers

# You can generate password hash by using this command in terminal:
#  printf "%q" $(mkpasswd -m sha256crypt your_pass_goes_here)

# Default root password is set to raspberry
ROOT_PASSWORD = "\$5\$/zE74HBS1sv6y/JR\$BvkKkw52Be/Yt.nyI8QzBmdx9mQ7mSSrJ/O96vOQvQ4"

# Default student password is set to student
STUDENT_PASSWORD = "\$5\$hClPjxDEMR7eGaXW\$3uJkQraDPdNsgeEDo3BPlX8Rh0ejrJn4O2c.bgBW1a9"

EXTRA_USERS_PARAMS = "\
        usermod -p '${ROOT_PASSWORD}' root; \
        useradd -p '${STUDENT_PASSWORD}' student; \
	"
