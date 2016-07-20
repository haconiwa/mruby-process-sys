module Process
  module GID
    extend self
    def eid; Sys.getegid; end
    def eid=(i); Sys.setegid(i); end
    def change_privilege(i)
      Sys.setegid(i)
      Sys.setgid(i)
    end

    def from_name(name)
      Pwgrp.getgrnam(name)
    end
  end

  module UID
    extend self
    def eid; Sys.geteuid; end
    def eid=(i); Sys.seteuid(i); end
    def change_privilege(i)
      Sys.seteuid(i)
      Sys.setuid(i)
    end

    def from_name(name)
      Pwgrp.getpwnam(name)
    end
  end
end
