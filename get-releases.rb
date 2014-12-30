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

repo = "stefansundin/altdrag"

page = 1
releases = []
total_downloads = 0

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

print "\n\n"
puts "Found #{releases.count} releases!"

open("README.md", 'wb') do |io|
  io.write <<-eos
# #{repo}

Release | Filename | Download Count
------- | -------- | --------------
eos

  releases.sort_by { |r| r["created_at"] }.each do |r|
    puts
    puts r["tag_name"]
    %x[mkdir "#{r["tag_name"]}"] unless File.directory?(r["tag_name"])

    open("#{r["tag_name"]}/README.md", 'wb') do |tag_io|
      tag_io.write <<-eos
# #{r["tag_name"]}

#{r["body"]}

Filename | Download Count
-------- | --------------
eos

      r["assets"].sort_by { |a| a["name"] }.each do |a|
        tag_io.write "#{a["name"]} | #{a["download_count"]}\n"
        io.write "#{r["tag_name"]} | #{a["name"]} | #{a["download_count"]}\n"
        total_downloads += a["download_count"]

        puts "- #{a["name"]}"
        path = "#{r["tag_name"]}/#{a["name"]}"
        unless File.exists?(path) and File.size(path) == a["size"]
          download(a["browser_download_url"], path)
        end
      end
    end
  end
  io.write "\nTotal downloads: #{total_downloads}\n"
end
