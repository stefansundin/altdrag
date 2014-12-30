#!/usr/bin/env ruby

require 'net/http'
require 'json'

def download(url, dest=nil, limit=10)
  # puts url
  raise ArgumentError, 'HTTP redirect too deep' if limit == 0

  uri = URI.parse(url)
  http = Net::HTTP.new(uri.host, uri.port)
  if uri.port == 443
    # http.verify_mode = OpenSSL::SSL::VERIFY_NONE
    http.use_ssl = true
  end
  request = Net::HTTP::Get.new(uri.request_uri)
  http.request(request) do |response|
    case response
    when Net::HTTPSuccess then
      open(dest, 'wb') do |io|
        response.read_body do |chunk|
          io.write chunk
        end
      end
    when Net::HTTPRedirection then
      download(response['location'], dest, limit-1)
    end
  end
end

def humanize(number)
  number.to_s.reverse.gsub(/(\d{3})(?=\d)/, '\\1,').reverse
end

page = 1
releases = []
filetype_downloads = {}
total_downloads = 0

repo = %x[git remote -v][/(?<=:)[^ ]+(?=\.git)/]
puts repo

print "Getting page "

while true
  print "#{page}.. "

  path = "page%02d.json" % page
  download("https://api.github.com/repos/#{repo}/releases?page=#{page}", path)
  entries = JSON.load File.read(path)
  break if entries.count == 0
  releases = releases + entries

  page += 1
end

print "\n"
puts "Found #{releases.count} releases!"

open("README.md", 'wb') do |readme|
  readme.write <<-eos
# #{repo}

## Downloads by release

Release | Downloads
------- | ---------
eos

  releases.sort_by { |r| r["created_at"] }.reverse!.each do |r|
    release_downloads = 0
    puts
    puts r["tag_name"]
    %x[mkdir "#{r["tag_name"]}"] unless File.directory?(r["tag_name"])

    open("#{r["tag_name"]}/README.md", 'wb') do |tag_readme|
      tag_readme.write <<-eos
# #{r["tag_name"]}

#{r["body"]}

## Downloads by filename

Filename | Download Count
-------- | --------------
eos

      r["assets"].sort_by { |a| a["name"] }.each do |a|
        tag_readme.write "#{a["name"]} | #{humanize(a["download_count"])}\n"
        total_downloads += a["download_count"]
        release_downloads += a["download_count"]

        ext = File.extname(a["name"])
        filetype_downloads[ext] ||= 0
        filetype_downloads[ext] += a["download_count"]

        puts "- #{a["name"]}"
        path = "#{r["tag_name"]}/#{a["name"]}"
        unless File.exists?(path) and File.size(path) == a["size"]
          download(a["browser_download_url"], path)
        end
      end
      readme.write "[#{r["tag_name"]}](#{r["tag_name"]}) | #{humanize(release_downloads)}\n"
    end
  end

  readme.write <<-eos

## Downloads by filetype

Filetype | Download Count
-------- | --------------
eos
  filetype_downloads.sort.each do |filetype, downloads|
    readme.write "#{filetype} | #{downloads}\n"
  end
  readme.write "\nTotal downloads: #{total_downloads}\n"
end
