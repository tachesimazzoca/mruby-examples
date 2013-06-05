class AppSetting
  class << self
    attr_accessor :settings

    def load(*targets)
      conf = AppConfig.new
      names = targets.map {|t| t.to_s.strip }
        .reject {|t| t.empty? || t == "common" }
        .unshift("common")
      names.each do |name|
        if AppSetting.settings.key?(name)
          AppSetting.settings[name].setup(conf)
        else
          conf.info "Unknown target #{name}"
        end
      end
      conf
    end

    def setting(target)
      name = target.to_s
      AppSetting.settings[name]
    end
  end

  def initialize(name, &block)
    @name, @initializer = name.to_s, block
    AppSetting.settings ||= {}
    AppSetting.settings[@name] = self
  end

  def setup(conf)
    conf.instance_eval(&@initializer)
  end
end

class AppConfig
  attr_accessor :messages

  def initialize
    @debug = false
    @timeout = 0
    @title = "Untitled"
    @messages = []
    @formatter = Proc.new {|s| s }
  end

  def debug(b=nil)
    @debug = b unless b.nil?
    @debug
  end

  def timeout(n=nil)
    @timeout = n.to_i if n
    @timeout
  end

  def title(str=nil)
    @title = str.to_s if str
    @title
  end

  def formatter(&block)
    @formatter = block
  end

  def info(str)
    @messages << @formatter.call(str)
  end
end

AppSetting.new :common do
  debug false
  timeout 10
  title "MRuby App"
  formatter do |s|
    "#{title} - #{s}"
  end
  info "Loaded :common"
end

AppSetting.new :production do
  debug false
  timeout 5
  info "Loaded :production"
end

AppSetting.new :development do
  debug true
  timeout (timeout * 3)
  title "[dev]#{title}"
  info "Loaded :development"
end
