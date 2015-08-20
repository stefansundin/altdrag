#!/usr/bin/env ruby

# note that when this script downloads a file, it also increases the download counter

require "httparty"
require "date"

class GithubParty
  include HTTParty
  base_uri "https://api.github.com"

  def self.get_all_pages(path)
    entries = []
    page = 1
    while true
      r = get "#{path}?page=#{page}"
      return nil if r.code == 404
      raise error(r) if not r.success?
      break if r.parsed_response.count == 0
      entries = entries + r.parsed_response
      page += 1
    end
    entries
  end
end

def humanize(number)
  number.to_s.reverse.gsub(/(\d{3})(?=\d)/, "\\1,").reverse
end

filetype_downloads = {}
total_downloads = 0

repo = %x[git remote -v][/(?<=:)[^ ]+(?=\.git)/]
puts "Repo: #{repo}"

print "Getting list of downloads..."
releases = GithubParty.get_all_pages "https://api.github.com/repos/#{repo}/releases"

open("releases.json", "wb") { |f| f.write releases.to_json }

puts
puts "Found #{releases.count} releases!"

open("README.md", "wb") do |readme|
  readme.write <<-eos
# #{repo}

## Downloads by release

Date       | Release | Downloads
---------- | ------- | ---------
eos

  releases.sort_by { |r| r["created_at"] }.reverse!.each do |r|
    release_downloads = 0
    puts
    puts r["tag_name"]
    %x[mkdir "#{r["tag_name"]}"] unless File.directory?(r["tag_name"])

    open("#{r["tag_name"]}/README.md", "wb") do |tag_readme|
      tag_readme.write <<-eos
# #{r["name"]}

#{r["body"].gsub("\r","")}

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
        download(a["browser_download_url"], path) unless File.exists?(path) and File.size(path) == a["size"]
      end
      date = Date.parse r["created_at"]
      readme.write "#{date.strftime("%Y-%m-%d")} | [#{r["tag_name"]}](#{r["tag_name"]}) | #{humanize(release_downloads)}\n"
    end
  end

  readme.write <<-eos

## Downloads by filetype

Filetype | Download Count
-------- | --------------
eos
  filetype_downloads.sort.reverse!.each do |filetype, downloads|
    readme.write "#{filetype} | #{downloads}\n"
  end
  readme.write "\nTotal downloads: #{total_downloads}\n"
end
